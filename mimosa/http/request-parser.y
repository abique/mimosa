%debug
%pure-parser
%start request
%defines
%error-verbose
%name-prefix "request_"
%output "request-parser.cc"

%lex-param {yyscan_t yyscanner}
%parse-param {yyscan_t yyscanner}
%parse-param {mimosa::http::Request & rq}

%{
#include <utility>

#include "request.hh"
#include "request-parser.hh"
#include "request-lexer.hh"

  static void yyerror(yyscan_t                /*yyscanner*/,
                      mimosa::http::Request & rq,
                      const char *            str)
  {
    ;
  }

%}

%union
{
  char *  text;
  int     ival;
  int64_t val64;
}

// first line
%token HEAD GET PUT DELETE TRACE OPTIONS CONNECT PATCH
%token <text> LOCATION
%token <ival> PROTO_MAJOR PROTO_MINOR

// keys
%token <text> KEY ATTR
%token KEY_ACCEPT_ENCODING
%token KEY_CONNECTION
%token KEY_COOKIE
%token KEY_CONTENT_LENGTH
%token KEY_CONTENT_TYPE
%token KEY_HOST
%token KEY_REFERRER
%token KEY_USER_AGENT

// values
%token <text> VALUE QUOTED_VALUE
%token <ival> VALUE_CONNECTION
%token <val64> VAL64
%token COMPRESS IDENTITY DEFLATE GZIP

%destructor { free($$); } <text>

%%

request: method LOCATION PROTO_MAJOR PROTO_MINOR kvs {
  rq.raw_location_ = $2;
  rq.proto_major_  = $3;
  rq.proto_minor_  = $4;
};

method:
  HEAD    { rq.method_ = mimosa::http::kMethodHead; }
| GET     { rq.method_ = mimosa::http::kMethodGet; }
| PUT     { rq.method_ = mimosa::http::kMethodPut; }
| DELETE  { rq.method_ = mimosa::http::kMethodDelete; }
| TRACE   { rq.method_ = mimosa::http::kMethodTrace; }
| OPTIONS { rq.method_ = mimosa::http::kMethodOptions; }
| CONNECT { rq.method_ = mimosa::http::kMethodConnect; }
| PATCH   { rq.method_ = mimosa::http::kMethodPatch; };

kvs: kv kvs | /* epsilon */ ;

kv:
  KEY VALUE { rq.unparsed_headers_.insert(std::make_pair($1, $2)); }
| KEY_ACCEPT_ENCODING accept_encodings
| KEY_CONNECTION VALUE_CONNECTION { rq.keep_alive_ = $2; }
| KEY_COOKIE cookie cookies
| KEY_CONTENT_LENGTH VAL64 { rq.content_length_ = $2; }
| KEY_CONTENT_TYPE VALUE { rq.content_type_ = $2; }
| KEY_HOST VALUE { rq.host_ = $2; }
| KEY_REFERRER VALUE { rq.referrer_ = $2; }
| KEY_USER_AGENT VALUE { rq.user_agent_ = $2; };

accept_encodings: /* epsilon */
| COMPRESS accept_encodings { rq.accept_encoding_ |= mimosa::http::kCodingCompress; }
| IDENTITY accept_encodings { rq.accept_encoding_ |= mimosa::http::kCodingIdentity; }
| DEFLATE  accept_encodings { rq.accept_encoding_ |= mimosa::http::kCodingDeflate; }
| GZIP     accept_encodings { rq.accept_encoding_ |= mimosa::http::kCodingGzip; };

cookies:
  /* epsilon */
| ';' cookie cookies;

cookie:
  ATTR { rq.cookies_.insert(std::make_pair($1, nullptr)); }
| ATTR '=' VALUE { rq.cookies_.insert(std::make_pair($1, $3)); }
| ATTR '=' QUOTED_VALUE { rq.cookies_.insert(std::make_pair($1, $3)); /*todo unescape*/ };

%%
