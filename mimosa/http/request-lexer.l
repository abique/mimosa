%option reentrant noyywrap batch 8bit
%option bison-bridge stack
%option warn ansi-definitions ansi-prototypes
%option prefix="mimosa_http_request_"
%option outfile="request-lexer.cc" header-file="request-lexer.hh"
/* %option nodefault */
%option align
%option fast

%{
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "request-parser.hh"

#define SAVE_TEXT                                \
  do {                                           \
    yylval->text = (char*)::malloc(yyleng + 1);  \
    if (yylval->text)                            \
      memcpy(yylval->text, yytext, yyleng + 1);  \
  } while (0)
%}

%x location
%x protover
%x proto_major
%x proto_minor
%x eol
%x key
%x value
%x value64
%x colon
%x value_accept_encoding
%x value_connection
%x value_cookie_attr
%x value_cookie_eq
%x value_cookie_value
%x value_cookie_str
%x value_cookie_sm

separtor [\(\)<>@,;:\\"/\[\]\?={} \t]
token    [^\(\)<>@,;:\\"/\[\]\?={} \t\r\n\v]+
spaces   [\t\v ]

%%

<INITIAL>(?i:HEAD)      BEGIN(location); return HEAD;
<INITIAL>(?i:GET)       BEGIN(location); return GET;
<INITIAL>(?i:PUT)       BEGIN(location); return PUT;
<INITIAL>(?i:DELETE)    BEGIN(location); return DELETE;
<INITIAL>(?i:TRACE)     BEGIN(location); return TRACE;
<INITIAL>(?i:OPTIONS)   BEGIN(location); return OPTIONS;
<INITIAL>(?i:CONNECT)   BEGIN(location); return CONNECT;
<INITIAL>(?i:PATCH)     BEGIN(location); return PATCH;

<location>[^\t\v\r\n ]+ BEGIN(protover); SAVE_TEXT; return LOCATION;

<protover>(?i:http)\/   BEGIN(proto_major);
<proto_major>[[:digit:]]+\.      BEGIN(proto_minor); return PROTO_MAJOR;
<proto_minor>[[:digit:]]+        BEGIN(eol); return PROTO_MINOR;


<key>(?i:Accept-Encoding){spaces}*:     BEGIN(value_accept_encoding); return KEY_ACCEPT_ENCODING;
<key>(?i:Connection){spaces}*:          BEGIN(value_connection); return KEY_CONNECTION;
<key>(?i:Cookie){spaces}*:              BEGIN(value_cookie_attr); return KEY_COOKIE;
<key>(?i:Content-Length){spaces}*:      BEGIN(value64); return KEY_CONTENT_LENGTH;
<key>(?i:Content-Type)                  BEGIN(colon); return KEY_CONTENT_TYPE;
<key>(?i:Host)                          BEGIN(colon); return KEY_HOST;
<key>(?i:Referr?er)                     BEGIN(colon); return KEY_REFERRER;
<key>(?i:User-Agent)                    BEGIN(colon); return KEY_USER_AGENT;
<key>[-_[:alnum:]]+                     BEGIN(colon); SAVE_TEXT; return KEY;

<colon>:                                BEGIN(value);

<value>[^[:space:]].*/\r\n      SAVE_TEXT; return VALUE;

<value_accept_encoding>(?i:compress) return COMPRESS;
<value_accept_encoding>(?i:identity) return IDENTITY;
<value_accept_encoding>(?i:deflate)  return DEFLATE;
<value_accept_encoding>(?i:gzip)     return GZIP;
<value_accept_encoding>;|[qQ]|=[01](.[0-9]{1,3})?|,

<value_cookie_attr>{token}      BEGIN(value_cookie_eq); SAVE_TEXT; return ATTR;
<value_cookie_eq>=              BEGIN(value_cookie_value); return '=';
<value_cookie_eq,value_cookie_sm>\; BEGIN(value_cookie_attr); return ';';
<value_cookie_value>{token}     BEGIN(value_cookie_sm); SAVE_TEXT; return VALUE;
<value_cookie_value>\"          yymore(); BEGIN(value_cookie_str);
<value_cookie_str>[^\\"]+       yymore();
<value_cookie_str>\\.           yymore();
<value_cookie_str>\"            BEGIN(value_cookie_sm); SAVE_TEXT; return QUOTED_VALUE;

<value_connection>(?i:keep-alive)  yylval->ival = 1; return VALUE_CONNECTION;
<value_connection>(?i:close)       yylval->ival = 0; return VALUE_CONNECTION;

<value64>[0-9]+         yylval->val64 = strtoll(yytext, 0, 0); return VAL64;

<*>{spaces}+            /* ignore */
<*>\r\n\r\n             yyterminate();
<*>\r\n                 BEGIN(key);
<*>.                    yyterminate();
%%
