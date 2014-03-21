#ifndef MIMOSA_URI_URL_HH
# define MIMOSA_URI_URL_HH

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
      bool parse(const std::string & url, std::string *parse_error);

      inline Scheme scheme() const { return scheme_; }
      inline const StringRef & host() const { return host_; }
      inline uint16_t port() const { return port_; }
      inline const StringRef & location() const { return location_; }
      inline const StringRef & query() const { return query_; }
      inline const StringRef & anchor() const { return anchor_; }

      inline const std::string & rawUrl() const { return raw_url_; }

    private:

      bool parseScheme(StringRef & in, std::string *parse_error);
      bool parseUserInfo(StringRef & in, std::string *parse_error);
      bool parseAuthority(StringRef & in, std::string *parse_error);
      bool parseHost(StringRef & in, std::string *parse_error);
      bool parsePort(StringRef & in, std::string *parse_error);
      void parseLocation(StringRef & in);
      void parseQuery(StringRef & in);
      void parseAnchor(StringRef & in);

      std::string	raw_url_;
      Scheme		scheme_;
      StringRef		host_;
      uint16_t		port_;
      StringRef		location_;
      StringRef		query_;
      StringRef		anchor_;
    };
  }
}

#endif /* !MIMOSA_URI_URL_HH */
