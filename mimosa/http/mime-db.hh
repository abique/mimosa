#pragma once

# include <string>
# include <unordered_map>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class MimeDb : private NonCopyable
    {
    public:
      static MimeDb & instance();

      std::string mimeType(const std::string & filename);

    private:
      MimeDb();

      void load();

      std::unordered_map<std::string, std::string> mime_types_;
    };
  }
}

