%option reentrant noyywrap batch 8bit
%option bison-bridge stack
%option warn ansi-definitions ansi-prototypes
%option prefix="mimosa_http_request_"
/* %option nodefault */
%option align
%option fast
%option extra-type="std::string *"

%{
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>

#include "request.hh"
#include "request-parser.hh"

#define SAVE_TEXT                                   \
  do {                                              \
    yylval->text = new std::string(yytext, yyleng); \
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
%x port
%x host
%x content_range_unit
%x content_range_start
%x content_range_dash
%x content_range_end
%x content_range_slash
%x content_range_length

separtor [\(\)<>@,;:\\"/\[\]\?={} \t]
token    [^\(\)<>@,;:\\"/\[\]\?={} \t\r\n\v]+
spaces   [\t\v ]
cookie_octet [^\r\r\n\v ;",\\]

%%

<INITIAL>(?i:HEAD)      BEGIN(location); return HEAD;
<INITIAL>(?i:GET)       BEGIN(location); return GET;
<INITIAL>(?i:POST)      BEGIN(location); return POST;
<INITIAL>(?i:PUT)       BEGIN(location); return PUT;
<INITIAL>(?i:DELETE)    BEGIN(location); return DELETE;
<INITIAL>(?i:TRACE)     BEGIN(location); return TRACE;
<INITIAL>(?i:OPTIONS)   BEGIN(location); return OPTIONS;
<INITIAL>(?i:CONNECT)   BEGIN(location); return CONNECT;
<INITIAL>(?i:PATCH)     BEGIN(location); return PATCH;
<INITIAL>(?i:PROPFIND)  BEGIN(location); return PROPFIND;
<INITIAL>(?i:PROPPATCH) BEGIN(location); return PROPPATCH;
<INITIAL>(?i:MKCOL)     BEGIN(location); return MKCOL;
<INITIAL>(?i:COPY)      BEGIN(location); return COPY;
<INITIAL>(?i:MOVE)      BEGIN(location); return MOVE;
<INITIAL>(?i:LOCK)      BEGIN(location); return LOCK;
<INITIAL>(?i:UNLOCK)    BEGIN(location); return UNLOCK;

<location>[^\t\v\r\n ]+ BEGIN(protover); SAVE_TEXT; return LOCATION;

<protover>(?i:http)\/   BEGIN(proto_major);
<proto_major>[[:digit:]]+\.      BEGIN(proto_minor); return PROTO_MAJOR;
<proto_minor>[[:digit:]]+        BEGIN(eol); return PROTO_MINOR;


<key>(?i:Accept-Encoding){spaces}*:     BEGIN(value_accept_encoding); return KEY_ACCEPT_ENCODING;
<key>(?i:Connection){spaces}*:          BEGIN(value_connection); return KEY_CONNECTION;
<key>(?i:Cookie){spaces}*:              BEGIN(value_cookie_attr); return KEY_COOKIE;
<key>(?i:Content-Length){spaces}*:      BEGIN(value64); return KEY_CONTENT_LENGTH;
<key>(?i:Content-Range){spaces}*:       BEGIN(content_range_unit); return KEY_CONTENT_RANGE;
<key>(?i:Content-Type)                  BEGIN(colon); return KEY_CONTENT_TYPE;
<key>(?i:Host){spaces}*:                BEGIN(host); return KEY_HOST;
<key>(?i:Referr?er)                     BEGIN(colon); return KEY_REFERRER;
<key>(?i:User-Agent)                    BEGIN(colon); return KEY_USER_AGENT;
<key>(?i:If-Modified-Since)             BEGIN(colon); return KEY_IF_MODIFIED_SINCE;
<key>[-_[:alnum:]]+                     BEGIN(colon); SAVE_TEXT; return KEY;

<colon>:                                BEGIN(value);

<host>{token}                           SAVE_TEXT; return HOST;
<host>:                                 BEGIN(port);

<port>[0-9]+                            yylval->ival = atoi(yytext); return PORT;

<value>[^[:space:]].*/\r\n      SAVE_TEXT; return VALUE;

<content_range_unit>bytes          BEGIN(content_range_start); yylval->val64 = 1; return RANGE_UNIT;
<content_range_start>[[:digit:]]+  BEGIN(content_range_dash); yylval->val64 = strtoll(yytext, nullptr, 0); return RANGE_START;
<content_range_dash>-              BEGIN(content_range_end);
<content_range_end>[[:digit:]]+    BEGIN(content_range_slash); yylval->val64 = strtoll(yytext, nullptr, 0); return RANGE_END;
<content_range_slash>\/            BEGIN(content_range_length);
<content_range_length>[[:digit:]]+ yylval->val64 = strtoll(yytext, nullptr, 0); return RANGE_LENGTH;
<content_range_length>\*           yylval->val64 = -1; return RANGE_LENGTH;

<value_accept_encoding>(?i:compress) return COMPRESS;
<value_accept_encoding>(?i:identity) return IDENTITY;
<value_accept_encoding>(?i:deflate)  return DEFLATE;
<value_accept_encoding>(?i:gzip)     return GZIP;
<value_accept_encoding>(?i:sdch)     return SDCH;
<value_accept_encoding>;|[qQ]|=[01](.[0-9]{1,3})?|,

<value_cookie_attr>{token}      BEGIN(value_cookie_eq); SAVE_TEXT; return ATTR;
<value_cookie_eq>=              BEGIN(value_cookie_value); return '=';
<value_cookie_eq,value_cookie_value,value_cookie_sm>\; BEGIN(value_cookie_attr); return ';';
<value_cookie_value>{cookie_octet}* BEGIN(value_cookie_sm); SAVE_TEXT; return VALUE;
<value_cookie_value>\"          BEGIN(value_cookie_str); yyextra->clear();
<value_cookie_str>[^\\"]+       yyextra->append(yytext, yyleng);
<value_cookie_str>\\.           yyextra->push_back(yytext[1]);
<value_cookie_str>\"            BEGIN(value_cookie_sm); yylval->text = new std::string(*yyextra); return VALUE;

<value_connection>,
<value_connection>(?i:TE)
<value_connection>(?i:keep-alive)  yylval->ival = 1; return VALUE_CONNECTION;
<value_connection>(?i:close)       yylval->ival = 0; return VALUE_CONNECTION;

<value64>[0-9]+         yylval->val64 = strtoll(yytext, 0, 0); return VAL64;

<*>{spaces}+            /* ignore */
<*>\r\n\r\n             yyterminate();
<*>\r\n                 BEGIN(key);
<*>.                    yyterminate();
%%
