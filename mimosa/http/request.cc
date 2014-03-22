#include "time.hh"
#include "log.hh"
#include "request.hh"
#include "request-parser.hh"
#include "request-lexer.hh"
#include "../uri/normalize-path.hh"
#include "../uri/parse-query.hh"
#include "../uri/percent-encoding.hh"
#include "../format/print.hh"

int mimosa_http_request_parse(yyscan_t scanner, mimosa::http::Request & request);

namespace mimosa
{
  namespace http
  {
    Request::Request()
    {
      clear();
    }

    void
    Request::clear()
    {
      method_ = kMethodGet;
      proto_major_ = 1;
      proto_minor_ = 1;
      raw_location_.clear();
      host_.clear();
      port_ = 80;
      location_normalized_ = false;
      location_.clear();
      query_parsed_ = false;
      query_.clear();
      accept_encoding_ = kCodingIdentity;
      keep_alive_ = false;
      cookies_.clear();
      content_length_ = 0;
      if_modified_since_ = 0;
      content_type_.clear();
      body_.clear();
      referrer_.clear();
      user_agent_.clear();
      unparsed_headers_.clear();
      content_range_start_ = 0;
      content_range_end_ = 0;
      content_range_length_ = 0;
      content_encoding_  = kCodingIdentity;
      transfer_encoding_ = kCodingIdentity;
    }

    bool
    Request::print(stream::Stream & stream) const
    {
      bool ok = true;
      ok = ok & format::print(stream, methodString(method_));
      ok = ok & format::printStatic(stream, " ");
      ok = ok & format::print(stream, url_.location());
      ok = ok & format::printStatic(stream, " HTTP/1.1\r\n");

      ok = ok & format::printStatic(stream, "Host: ");
      ok = ok & format::print(stream, host_);
      ok = ok & format::printStatic(stream, "\r\n");

      // end
      ok = ok & format::printStatic(stream, "\r\n");
      return ok;
    }


#define PARSE(Name, Const, Scan)                                        \
    bool                                                                \
    Request::Name(Const char * data, size_t size)                       \
    {                                                                   \
      http_log->debug(                                                  \
        "parsing request =================================\n"           \
        "%s"                                                            \
        "=================================================",            \
        data);                                                          \
      clear();                                                          \
      std::string quoted_buffer;                                        \
      yyscan_t    scanner;                                              \
      if (mimosa_http_request_lex_init_extra(&quoted_buffer, &scanner)) \
        return false;                                                   \
      mimosa_http_request__##Scan(data, size, scanner);                 \
      bool succeed = !mimosa_http_request_parse(scanner, *this);        \
      mimosa_http_request_lex_destroy(scanner);                         \
      return succeed;                                                   \
    }

    PARSE(parse, const, scan_bytes)
    PARSE(parseZeroCopy, , scan_buffer)

    const std::string &
    Request::location() const
    {
      if (location_normalized_)
        return location_;
      location_normalized_ = true;

      auto pos = raw_location_.find_first_of("?#");
      if (pos == std::string::npos)
        pos = raw_location_.size();
      std::string decoded;
      uri::percentDecode(raw_location_.data(), pos, &decoded, uri::kRfc2396);
      uri::normalizePath(decoded.data(), decoded.size(), &location_);
      return location_;
    }

    void
    Request::parseIfModifiedSince(const std::string & value)
    {
      if_modified_since_ = time(value);
    }

    const kvs &
    Request::query() const
    {
      if (query_parsed_)
        return query_;
      query_parsed_ = true;

      auto start = raw_location_.find_first_of('?');
      if (start == std::string::npos)
        return query_;
      ++start;

      auto end = raw_location_.find_first_of('#');
      if (end == std::string::npos)
        end = raw_location_.size();
      uri::parseQuery(raw_location_.data() + start, end - start, &query_);
      return query_;
    }

    const std::string &
    Request::queryGet(const std::string & key) const
    {
      static const std::string empty;

      auto & q = query();
      auto it = q.find(key);
      if (it == q.end())
        return empty;
      return it->second;
    }

    void
    Request::setUrl(const uri::Url & url)
    {
      url_ = url;
      setRawLocation(url.rawUrl());
      setHost(url.host());
      setPort(url.port());
    }
  }
}
