#include "request.hh"
#include "request-parser.hh"
#include "request-lexer.hh"

int mimosa_http_request_parse(yyscan_t scanner, mimosa::http::Request & request);

namespace mimosa
{
  namespace http
  {
    Request::Request()
      : method_(kMethodGet),
        proto_major_(1),
        proto_minor_(1),
        accept_encoding_(kCodingIdentity),
        keep_alive_(false),
        content_length_(0)
    {
    }

#define PARSE(Name, Const, Scan)                                        \
    bool                                                                \
    Request::Name(Const char * data, size_t size)                       \
    {                                                                   \
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
  }
}
