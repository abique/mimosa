#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <sstream>
#include <algorithm>

#include "../string-ref.hh"
#include "../stream/copy.hh"
#include "../stream/direct-fd-stream.hh"
#include "../stream/buffered-stream.hh"
#include "../fs/rm.hh"
#include "../json/encoder.hh"
#include "fs-handler.hh"
#include "error-handler.hh"
#include "mime-db.hh"
#include "log.hh"

namespace mimosa
{
  namespace http
  {
    FsHandler::FsHandler(const std::string & root, int nskip)
      : root_(root),
        nskip_(nskip),
        can_readdir_(false),
        can_head_(true),
        can_get_(true),
        can_put_(false),
        can_delete_(false),
        use_xattr_(false)
    {
    }

    std::string
    FsHandler::checkPath(RequestReader & request) const
    {
      StringRef path(request.location());
      for (auto nskip = nskip_; nskip > 0; --nskip)
      {
        auto pos = path.find('/', 1);
        if (pos == StringRef::npos)
        {
          if (nskip == 1 && path.size() > 1) // our location is root and is not terminated by '/'
          {
            path = StringRef("/");
            break;
          }
          else
            return "";
        }
        path = path.substr(pos);
      }

      // TODO: check every component for symlinks going out of root.

      std::string real_path(root_);
      real_path.append(path.data(), path.size());
      return real_path;
    }

    bool
    FsHandler::head(RequestReader & request, ResponseWriter & response) const
    {
      if (!can_head_)
        return ErrorHandler::basicResponse(request, response, kStatusForbidden);

      std::string real_path = checkPath(request);
      if (real_path.empty()) {
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
      }

      struct stat st;
      if (::stat(real_path.c_str(), &st))
        return ErrorHandler::basicResponse(request, response, kStatusNotFound);
      return true;
    }

    bool
    FsHandler::get(RequestReader & request, ResponseWriter & response) const
    {
      if (!can_get_)
        return ErrorHandler::basicResponse(request, response, kStatusForbidden);

      std::string real_path = checkPath(request);
      if (real_path.empty()) {
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
      }

      struct stat st;
      if (::stat(real_path.c_str(), &st))
        return ErrorHandler::basicResponse(request, response, kStatusNotFound);

      if (S_ISREG(st.st_mode)) {
        if (use_xattr_) {
          char xattr_buffer[128];
          getxattr(real_path.c_str(), "user.Content-Type", xattr_buffer, sizeof (xattr_buffer));
          xattr_buffer[sizeof (xattr_buffer) - 1] = '\0';
          response.setContentType(xattr_buffer);
        }
        return streamFile(request, response, real_path, st);
      } else if (S_ISDIR(st.st_mode) && can_readdir_)
        return readDir(request, response, real_path);
      return ErrorHandler::basicResponse(request, response, kStatusForbidden);
    }

    bool
    FsHandler::put(RequestReader & request, ResponseWriter & response) const
    {
      if (!can_put_)
        return ErrorHandler::basicResponse(request, response, kStatusForbidden);

      std::string real_path = checkPath(request);
      if (real_path.empty()) {
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
      }

      stream::DirectFdStream out;
      if (!out.open(real_path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0666))
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);

      if (stream::copy(request, out) < 0)
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);

      if (use_xattr_)
        setxattr(real_path.c_str(), "user.Content-Type", request.contentType().c_str(),
                 request.contentType().size() + 1, 0);
      return true;
    }

    bool
    FsHandler::del(RequestReader & request, ResponseWriter & response) const
    {
      if (!can_delete_)
        return ErrorHandler::basicResponse(request, response, kStatusForbidden);

      std::string real_path = checkPath(request);
      if (real_path.empty()) {
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
      }

      if (!fs::rm(real_path, false, false)) {
        if (errno == ENOENT)
          return ErrorHandler::basicResponse(request, response, kStatusNotFound);
        if (errno == EISDIR)
          return ErrorHandler::basicResponse(request, response, kStatusBadRequest);
        if (errno == EPERM || errno == EACCES)
          return ErrorHandler::basicResponse(request, response, kStatusForbidden);
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
      }
      return true;
    }

    bool
    FsHandler::streamFile(RequestReader &     request,
                          ResponseWriter &    response,
                          const std::string & real_path)
    {
      struct stat st;

      if (stat(real_path.c_str(), &st))
        return false;
      return streamFile(request, response, real_path, st);
    }

    bool
    FsHandler::streamFile(RequestReader &     request,
                          ResponseWriter &    response,
                          const std::string & real_path,
                          struct stat &       st)
    {
      if (request.hasIfModifiedSince() && request.ifModifiedSince() >= st.st_mtime)
      {
        response.setContentLength(0);
        response.setStatus(kStatusNotModified);
        response.sendHeader();
        return true;
      }

      int fd = ::open(real_path.c_str(), O_RDONLY, 0644);
      if (fd < 0)
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);

      int64_t length = st.st_size;

      // XXX: to use sendfile, we need to remove compression, so we also have
      // to check if the file we're going to send need to be compressed (like .avi,
      // .gif, .zip, ...)

      if (response.contentEncoding() == kCodingIdentity) {
        response.setContentLength(st.st_size);

        if (request.hasContentRange()) {
          if ((request.contentRangeLength() > 0 &&
               request.contentRangeLength() != st.st_size) ||
              request.contentRangeStart() >= request.contentRangeEnd() ||
              request.contentRangeEnd() >= st.st_size) {
            ::close(fd);
            return ErrorHandler::basicResponse(request, response, kStatusRequestedRangeNotSatisfiable);
          }

          if (request.contentRangeStart() > 0)
            if (::lseek64(fd, request.contentRangeStart(), SEEK_SET) != request.contentRangeStart()) {
              ::close(fd);
              return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
            }

          length = request.contentRangeEnd() - request.contentRangeStart();
          response.setStatus(kStatusPartialContent);
          response.setContentRange(request.contentRangeStart(),
                                   request.contentRangeStart(),
                                   st.st_size);
        }
      }

      if (response.contentType().empty())
        response.setContentType(MimeDb::instance().mimeType(real_path));
      response.setLastModified(st.st_mtime);
      response.sendHeader();

      stream::DirectFdStream file(fd);
      stream::DirectFdStream *sock = response.directFdStream();

      int64_t ret;
      if (sock)
        ret = stream::copy(file, *sock, length);
      else
        ret = stream::copy(file, response, length);
      return ret == st.st_size;
    }

    bool
    FsHandler::readDir(RequestReader &     request,
                       ResponseWriter &    response,
                       const std::string & real_path) const
    {
      auto & format = request.queryGet("format");

      if (format == "json")
        return readDirJson(request, response, real_path);
      return readDirHtml(request, response, real_path);
    }

    bool
    FsHandler::readDirHtml(RequestReader &     request,
                           ResponseWriter &    response,
                           const std::string & real_path) const
    {
      DIR * dir = ::opendir(real_path.c_str());
      if (!dir)
        return ErrorHandler::basicResponse(request, response, kStatusNotFound);

      response.setContentType("text/html");
      response.sendHeader();
      assert(response.transferEncoding() == kCodingChunked);

      std::ostringstream os;

      os << "<!DOCTYPE html><html><head>"
        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>"
        "<title>" << request.location() << "</title></head>"
        "<body><h1>Directory listing: " << request.location() << "</h1><hr/>";

      std::vector<std::string> directories;
      std::vector<std::string> files;

      struct dirent * entry;
      while ((entry = ::readdir(dir)))
      {
        std::ostringstream tmp;
        tmp << real_path << '/' << entry->d_name;

        std::string file_path(tmp.str());

        struct ::stat st;
        if (::stat(file_path.c_str(), &st))
          continue;

        if (S_ISDIR(st.st_mode))
          directories.push_back(entry->d_name);
        else
          files.push_back(entry->d_name);
      }

      std::sort(directories.begin(), directories.end());
      std::sort(files.begin(), files.end());

      for (auto it = directories.begin(); it != directories.end(); ++it)
        os << "<a href=\"" << *it << "/\">" << *it << "/</a><br/>";

      for (auto it = files.begin(); it != files.end(); ++it)
        os << "<a href=\"" << *it << "\">" << *it << "</a><br/>";

      os << "</body></html>";

      ::closedir(dir);
      std::string data(os.str());
      response.write(data.data(), data.size());
      return true;
    }

    bool
    FsHandler::readDirJson(RequestReader &     request,
                           ResponseWriter &    response,
                           const std::string & real_path) const
    {
      DIR * dir = ::opendir(real_path.c_str());
      if (!dir)
        return ErrorHandler::basicResponse(request, response, kStatusNotFound);

      response.setContentType("application/json");
      response.sendHeader();

      stream::Stream::Ptr buf(new stream::BufferedStream(&response));
      json::Encoder enc(buf);

      enc.startArray();
      struct dirent * entry;
      while ((entry = ::readdir(dir)))
      {
        std::ostringstream tmp;
        tmp << real_path << '/' << entry->d_name;

        std::string file_path(tmp.str());
        struct ::stat st;
        if (::stat(file_path.c_str(), &st))
          continue;

        enc.startObject();

        enc.pushString("name");
        enc.pushString(entry->d_name);

        enc.pushString("size");
        enc.pushNumber(st.st_size);

        enc.pushString("type");
        if (S_ISDIR(st.st_mode))
          enc.pushString("dir");
        else if (S_ISREG(st.st_mode))
          enc.pushString("reg");
        else if (S_ISLNK(st.st_mode))
          enc.pushString("lnk");
        else if (S_ISSOCK(st.st_mode))
          enc.pushString("sock");
        else if (S_ISFIFO(st.st_mode))
          enc.pushString("fifo");
        else if (S_ISCHR(st.st_mode))
          enc.pushString("chr");
        else if (S_ISBLK(st.st_mode))
          enc.pushString("blk");
        else
          enc.pushString("???");

        enc.endObject();
      }
      enc.endArray();
      ::closedir(dir);
      return true;
    }
  }
}
