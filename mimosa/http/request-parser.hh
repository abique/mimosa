/* A Bison parser, made by GNU Bison 2.6.2.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef MIMOSA_HTTP_REQUEST_HOME_ABIQUE_DEVELOP_MIMOSA_MIMOSA_HTTP_REQUEST_PARSER_HH
# define MIMOSA_HTTP_REQUEST_HOME_ABIQUE_DEVELOP_MIMOSA_MIMOSA_HTTP_REQUEST_PARSER_HH
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int mimosa_http_request_debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     HEAD = 258,
     GET = 259,
     POST = 260,
     PUT = 261,
     DELETE = 262,
     TRACE = 263,
     OPTIONS = 264,
     CONNECT = 265,
     PATCH = 266,
     LOCATION = 267,
     PROTO_MAJOR = 268,
     PROTO_MINOR = 269,
     KEY = 270,
     ATTR = 271,
     KEY_ACCEPT_ENCODING = 272,
     KEY_CONNECTION = 273,
     KEY_COOKIE = 274,
     KEY_CONTENT_RANGE = 275,
     KEY_CONTENT_LENGTH = 276,
     KEY_CONTENT_TYPE = 277,
     KEY_HOST = 278,
     KEY_REFERRER = 279,
     KEY_USER_AGENT = 280,
     KEY_IF_MODIFIED_SINCE = 281,
     VALUE = 282,
     HOST = 283,
     VALUE_CONNECTION = 284,
     PORT = 285,
     VAL64 = 286,
     RANGE_UNIT = 287,
     RANGE_START = 288,
     RANGE_END = 289,
     RANGE_LENGTH = 290,
     COMPRESS = 291,
     IDENTITY = 292,
     DEFLATE = 293,
     GZIP = 294,
     SDCH = 295
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2049 of yacc.c  */
#line 32 "/home/abique/develop/mimosa/mimosa/http/request-parser.y"

  std::string * text;
  int           ival;
  int64_t       val64;


/* Line 2049 of yacc.c  */
#line 104 "/home/abique/develop/mimosa/mimosa/http/request-parser.hh"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int mimosa_http_request_parse (void *YYPARSE_PARAM);
#else
int mimosa_http_request_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int mimosa_http_request_parse (void * yyscanner, mimosa::http::Request & rq);
#else
int mimosa_http_request_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !MIMOSA_HTTP_REQUEST_HOME_ABIQUE_DEVELOP_MIMOSA_MIMOSA_HTTP_REQUEST_PARSER_HH  */
