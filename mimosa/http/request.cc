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

    bool
    Request::parse(char * data, size_t size)
    {
      yyscan_t scanner;
      if (mimosa_http_request_lex_init(&scanner))
        return false;
      mimosa_http_request__scan_buffer(data, size, scanner);
      bool succeed = !mimosa_http_request_parse(scanner, *this);
      mimosa_http_request_lex_destroy(scanner);
      return succeed;
    }
  }
}
