%debug
%pure-parser
%start request
%defines
%error-verbose
%name-prefix "mimosa_http_request_"

%lex-param {void * yyscanner}
%parse-param {void * yyscanner}
%parse-param {mimosa::http::Request & rq}

%{
#include <stdio.h>
#include <utility>
#include <string>

#include "request.hh"
#include "request-parser.hh"
#include "request-lexer.hh"

  static void yyerror(void *                  /*yyscanner*/,
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
%token PROPFIND PROPPATCH MKCOL COPY MOVE LOCK UNLOCK
%token MIMOSA_SYMLINK
%token <text> LOCATION
%token <ival> PROTO_MAJOR PROTO_MINOR

// keys
%token <text> KEY ATTR
%token KEY_ACCEPT_ENCODING
%token KEY_CONNECTION
%token KEY_COOKIE
%token KEY_CONTENT_RANGE
%token KEY_CONTENT_LENGTH
%token KEY_CONTENT_TYPE
%token KEY_HOST
%token KEY_REFERRER
%token KEY_USER_AGENT
%token KEY_IF_MODIFIED_SINCE
%token KEY_DESTINATION
%token KEY_RANGE

// values
%token <text> VALUE HOST
%token <ival> VALUE_CONNECTION PORT
%token <val64> VAL64
%token <val64> RANGE_UNIT RANGE_START RANGE_END RANGE_LENGTH
%token COMPRESS IDENTITY DEFLATE GZIP SDCH

%destructor { delete $$; } <text>

%%

request: method LOCATION PROTO_MAJOR PROTO_MINOR kvs {
  rq.setRawLocation(std::move(*$2)); delete $2;
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
| PROPFIND { rq.setMethod(mimosa::http::kMethodPropfind); }
| PROPPATCH { rq.setMethod(mimosa::http::kMethodProppatch); }
| MKCOL { rq.setMethod(mimosa::http::kMethodMkcol); }
| COPY { rq.setMethod(mimosa::http::kMethodCopy); }
| MOVE { rq.setMethod(mimosa::http::kMethodMove); }
| LOCK { rq.setMethod(mimosa::http::kMethodLock); }
| UNLOCK { rq.setMethod(mimosa::http::kMethodUnlock); }
| MIMOSA_SYMLINK { rq.setMethod(mimosa::http::kMethodMimosaSymlink); };

kvs: kv kvs | /* epsilon */ ;

kv:
  KEY VALUE { rq.addHeader(*$1, *$2); delete $1; delete $2; }
| KEY_ACCEPT_ENCODING accept_encodings
| KEY_CONNECTION VALUE_CONNECTION { rq.setKeepAlive($2); }
| KEY_COOKIE cookies
| KEY_CONTENT_LENGTH VAL64 { rq.setContentLength($2); }
| KEY_CONTENT_TYPE VALUE { rq.setContentType(std::move(*$2)); delete $2; }
| KEY_HOST HOST PORT { rq.setHost(std::move(*$2)); delete $2; rq.setPort($3); }
| KEY_HOST HOST { rq.setHost(std::move(*$2)); delete $2; }
| KEY_REFERRER VALUE { rq.setReferrer(std::move(*$2)); delete $2; }
| KEY_USER_AGENT VALUE { rq.setUserAgent(std::move(*$2)); delete $2; };
| KEY_IF_MODIFIED_SINCE VALUE { rq.parseIfModifiedSince(std::move(*$2)); delete $2; }
| KEY_DESTINATION VALUE { rq.setDestination(std::move(*$2)); delete $2; }
| KEY_CONTENT_RANGE RANGE_UNIT RANGE_START RANGE_END RANGE_LENGTH {
    rq.setContentRange($2 * $3, $2 * $4, $2 * $5);
}
| KEY_RANGE RANGE_UNIT '=' byte_range_set;

accept_encodings: /* epsilon */
| COMPRESS accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingCompress); }
| IDENTITY accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingIdentity); }
| DEFLATE  accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingDeflate); }
| GZIP     accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingGzip); }
| SDCH     accept_encodings { rq.setAcceptEncoding(rq.acceptEncoding() | mimosa::http::kCodingSdch); };

cookies:
/* epsilon */
| cookie
| cookie ';' cookies ;

cookie:
ATTR { rq.addCookie(*$1, ""); delete $1; }
| ATTR '=' { rq.addCookie(*$1, ""); delete $1; };
| ATTR '=' VALUE { rq.addCookie(*$1, *$3); delete $1; delete $3; };

byte_range_set:
byte_range
| byte_range ',' byte_range_set;

byte_range:
VAL64 '-' VAL64 {
  mimosa::http::ByteRange br;
  br.type_  = mimosa::http::ByteRange::kRange;
  br.start_ = $1;
  br.end_   = $3;
  rq.addRange(br);
} | VAL64 '-' {
  mimosa::http::ByteRange br;
  br.type_  = mimosa::http::ByteRange::kStart;
  br.start_ = $1;
  br.end_   = 0;
  rq.addRange(br);
} | '-' VAL64 {
  mimosa::http::ByteRange br;
  br.type_  = mimosa::http::ByteRange::kSuffix;
  br.start_ = 0;
  br.end_   = $2;
  rq.addRange(br);
};

%%
