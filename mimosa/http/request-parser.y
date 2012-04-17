%debug
%pure-parser
%start request
%defines
%error-verbose
%name-prefix "mimosa_http_request_"

%lex-param {yyscan_t yyscanner}
%parse-param {yyscan_t yyscanner}
%parse-param {mimosa::http::Request & rq}

%{
#include <stdio.h>
#include <utility>
#include <string>

#include "request.hh"
#include "request-parser.hh"
#include "request-lexer.hh"

  static void yyerror(yyscan_t                /*yyscanner*/,
                      mimosa::http::Request & /*rq*/,
                      const char *            str)
  {
    if (str)
      printf("request parse error: %s\n", str);
  }

%}

%union
{
  std::string * text;
  int           ival;
  int64_t       val64;
}

// first line
%token HEAD GET POST PUT DELETE TRACE OPTIONS CONNECT PATCH
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
%token HOST
%token PORT

// values
%token <text> VALUE HOST
%token <ival> VALUE_CONNECTION PORT
%token <val64> VAL64
%token COMPRESS IDENTITY DEFLATE GZIP SDCH

%destructor { delete $$; } <text>

%%

request: method LOCATION PROTO_MAJOR PROTO_MINOR kvs {
  rq.setRawLocation(*$2); delete $2;
  rq.setProto($3, $4);
};

method:
  HEAD    { rq.setMethod(mimosa::http::kMethodHead); }
| GET     { rq.setMethod(mimosa::http::kMethodGet); }
| POST    { rq.setMethod(mimosa::http::kMethodPost); }
| PUT     { rq.setMethod(mimosa::http::kMethodPut); }
| DELETE  { rq.setMethod(mimosa::http::kMethodDelete); }
| TRACE   { rq.setMethod(mimosa::http::kMethodTrace); }
| OPTIONS { rq.setMethod(mimosa::http::kMethodOptions); }
| CONNECT { rq.setMethod(mimosa::http::kMethodConnect); }
| PATCH   { rq.setMethod(mimosa::http::kMethodPatch); };

kvs: kv kvs | /* epsilon */ ;

kv:
  KEY VALUE { rq.addHeader(*$1, *$2); delete $1; delete $2; }
| KEY_ACCEPT_ENCODING accept_encodings
| KEY_CONNECTION VALUE_CONNECTION { rq.setKeepAlive($2); }
| KEY_COOKIE cookies
| KEY_CONTENT_LENGTH VAL64 { rq.setContentLength($2); }
| KEY_CONTENT_TYPE VALUE { rq.setContentType(*$2); delete $2; }
| KEY_HOST HOST PORT { rq.setHost(*$2); delete $2; rq.setPort($3); }
| KEY_HOST HOST { rq.setHost(*$2); delete $2; }
| KEY_REFERRER VALUE { rq.setReferrer(*$2); delete $2; }
| KEY_USER_AGENT VALUE { rq.setUserAgent(*$2); delete $2; };

accept_encodings: /* epsilon */
| COMPRESS accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingCompress); }
| IDENTITY accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingIdentity); }
| DEFLATE  accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingDeflate); }
| GZIP     accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingGzip); }
| SDCH     accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingSdch); };

cookies:
/* epsilon */
| cookie
| cookie ';'
| cookie ';' cookies ;

cookie:
  ATTR { rq.addCookie(*$1, ""); delete $1; }
| ATTR '=' { rq.addCookie(*$1, ""); delete $1; };
| ATTR '=' VALUE { rq.addCookie(*$1, *$3); delete $1; delete $3; };

%%
