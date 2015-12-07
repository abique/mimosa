#include <fstream>
#include <sstream>

#include "mime-db.hh"

namespace mimosa
{
  namespace http
  {
    MimeDb::MimeDb()
      : mime_types_()
    {
      load();
    }

    std::string
    MimeDb::mimeType(const std::string & filename)
    {
      std::string ext;
      auto pos = filename.rfind('.');

      if (pos != std::string::npos)
        ext = filename.substr(pos + 1);
      else
        ext = filename;

      for (auto it = ext.begin(); it != ext.end(); ++it)
        *it = ::tolower(*it);

      auto mime_type = mime_types_.find(ext);
      if (mime_type == mime_types_.end())
        return "application/octet-stream";
      return mime_type->second;
    }

    MimeDb &
    MimeDb::instance()
    {
      static MimeDb mime_db;
      return mime_db;
    }

    void
    MimeDb::load()
    {
      std::ifstream in("/etc/mime.types");

      std::string line;

#define DEF(Ext, Type) mime_types_[Ext] = Type;

      DEF("aac", "audio/x-aac");
      DEF("c", "text/x-c");
      DEF("cc", "text/x-c++src");
      DEF("cpp", "text/x-c++src");
      DEF("css", "text/css");
      DEF("css", "text/css");
      DEF("cxx", "text/x-c++src");
      DEF("flac", "audio/x-flac");
      DEF("gif", "image/gif");
      DEF("h", "text/x-c");
      DEF("hh", "text/x-c++hdr");
      DEF("hpp", "text/x-c++hdr");
      DEF("htm", "text/html");
      DEF("html", "text/html");
      DEF("hxx", "text/x-c++hdr");
      DEF("ico", "image/x-icon");
      DEF("jpe", "image/jpeg");
      DEF("jpeg", "image/jpeg");
      DEF("jpg", "image/jpeg");
      DEF("js", "application/javascript");
      DEF("json", "application/json");
      DEF("midi", "audio/midi");
      DEF("mkv", "video/x-matroska");
      DEF("mp3", "audio/mpeg");
      DEF("mp4", "video/mp4");
      DEF("mp4", "video/mpg4");
      DEF("mp4a", "audio/mp4");
      DEF("oga", "audio/x-ogg");
      DEF("ogg", "audio/x-ogg");
      DEF("ogv", "video/x-ogg");
      DEF("pdf", "application/pdf");
      DEF("png", "image/png");
      DEF("svg", "image/svg+xml");
      DEF("svgz", "image/svg+xml");
      DEF("torrent", "application/x-bittorrent");
      DEF("text", "text/plain");
      DEF("txt", "text/plain");
      DEF("wav", "audio/x-wav");
      DEF("weba", "audio/webm");
      DEF("webm", "video/webm");
      DEF("xml", "text/xml");

      while (std::getline(in, line))
      {
        auto pos = line.find('#');
        if (pos != std::string::npos)
          line.erase(pos);

        std::istringstream line_in(line);

        std::string mime_type;
        line_in >> mime_type;

        if (mime_type.empty())
          continue;

        std::string ext;
        while (line_in >> ext)
          mime_types_[ext] = mime_type;
      }
    }
  }
}
