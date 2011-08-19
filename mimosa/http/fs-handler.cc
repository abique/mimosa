#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <sstream>

#include "../string/string-ref.hh"
#include "../stream/copy.hh"
#include "../stream/fd-stream.hh"
#include "fs-handler.hh"
#include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    FsHandler::FsHandler(const std::string & root, int nskip, bool enable_readdir)
      : root_(root),
        nskip_(nskip),
        can_readdir_(enable_readdir)
    {
    }

    bool
    FsHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      string::StringRef path(request.location());
      for (auto nskip = nskip_; nskip > 0; --nskip)
      {
        auto pos = path.find('/', 1);
        if (pos == string::StringRef::npos)
        {
          if (nskip == 1 && path.size() > 1) // our location is root and is not terminated by '/'
          {
            path = string::StringRef("/");
            break;
          }
          else
            // TODO: error reporting
            return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
        }
        path = path.substr(pos);
      }
      // TODO: check every component for symlinks going out of root.

      std::string real_path(root_);
      real_path.append(path.data(), path.size());

      struct stat st;
      if (::stat(real_path.c_str(), &st))
        return ErrorHandler::basicResponse(request, response, kStatusNotFound);

      if (S_ISREG(st.st_mode))
        return streamFile(request, response, real_path, st);
      else if (S_ISDIR(st.st_mode) && can_readdir_)
        return readDir(request, response, real_path);
      return ErrorHandler::basicResponse(request, response, kStatusForbidden);
    }

    bool
    FsHandler::streamFile(RequestReader &     request,
                          ResponseWriter &    response,
                          const std::string & real_path,
                          struct stat &       st) const
    {
      response.status_ = kStatusOk;
      response.content_length_ = st.st_size;
      response.sendHeader(0);

      auto file = stream::DirectFdStream::openFile(real_path.c_str(), O_RDONLY, 0644);
      if (!file)
        return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
      int64_t ret = stream::copy(*file, response, st.st_size);
      return ret == st.st_size;
    }

    bool
    FsHandler::readDir(RequestReader &     request,
                       ResponseWriter &    response,
                       const std::string & real_path) const
    {
      DIR * dir = ::opendir(real_path.c_str());
      if (!dir)
        return ErrorHandler::basicResponse(request, response, kStatusNotFound);

      std::ostringstream os;

      os << "<html><head></head><body><h1>Directory listing: " << request.location()
         << "</h1><hr/>";

      struct dirent * entry;
      while ((entry = ::readdir(dir)))
      {
        switch (entry->d_type)
        {
        case DT_BLK:  os << "[BLK] ";     break;
        case DT_CHR:  os << "[CHR] ";     break;
        case DT_DIR:  os << "[DIR] ";     break;
        case DT_FIFO: os << "[FIFO] ";    break;
        case DT_LNK:  os << "[LNK] ";     break;
        case DT_REG:  os << "[REG] ";     break;
        case DT_SOCK: os << "[SOCK] ";    break;
        case DT_UNKNOWN:
        default:      os << "[UNKNOWN] "; break;
        }
        os << "<a href=\"" << entry->d_name
           << (entry->d_type == DT_DIR ? "/" : "")
           << "\">" << entry->d_name
           << (entry->d_type == DT_DIR ? "/" : "")<< "</a><br/>";
      }
      os << "</body></html>";

      ::closedir(dir);
      std::string data(os.str());
      response.write(data.data(), data.size());
      response.content_type_ = "text/html";
      response.status_ = kStatusOk;
      return true;
    }
  }
}
