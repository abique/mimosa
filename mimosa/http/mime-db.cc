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
        return "application/binary";
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
