#pragma once

# include <cstdint>

# include "../ref-countable.hh"
# include "../string-ref.hh"
# include "scheme.hh"

namespace mimosa
{
  namespace uri
  {
    class Url : public RefCountable<Url>
    {
    public:
      Url();
      Url(const std::string & url);
      Url(const Url &url);

      Url &operator=(const Url &url);

      bool parse(const std::string & url, std::string *parse_error);

      inline Scheme scheme() const { return scheme_; }
      inline const StringRef & host() const { return host_; }
      inline uint16_t port() const { return port_; }
      inline const StringRef & path() const { return path_; }
      inline const StringRef & query() const { return query_; }
      inline const StringRef & anchor() const { return anchor_; }
      inline StringRef location() const {
        return StringRef(path_.begin());
      }

      inline const std::string & rawUrl() const { return raw_url_; }

    private:

      bool parseScheme(StringRef & in, std::string *parse_error);
      bool parseUserInfo(StringRef & in, std::string *parse_error);
      bool parseAuthority(StringRef & in, std::string *parse_error);
      bool parseHost(StringRef & in, std::string *parse_error);
      bool parsePort(StringRef & in, std::string *parse_error);
      void parsePath(StringRef & in);
      void parseQuery(StringRef & in);
      void parseAnchor(StringRef & in);

      std::string	raw_url_;
      Scheme		scheme_{kSchemeUnknown};
      StringRef		host_;
      uint16_t		port_{0};
      StringRef		path_;
      StringRef		query_;
      StringRef		anchor_;
    };
  }
}

