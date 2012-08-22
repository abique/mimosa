#include <gtest/gtest.h>

#include "../request.hh"

namespace mimosa
{
  namespace http
  {
    namespace
    {
#define METHOD_TEST(Meth)                               \
      TEST(RequestParser, Meth##Simple)                 \
      {                                                 \
        char str[] = #Meth " / HTTP/1.0\r\n\r\n";       \
        Request rq;                                     \
        EXPECT_EQ(1, rq.parse(str, sizeof (str)));      \
      }

      METHOD_TEST(GET)
      METHOD_TEST(HEAD)
      METHOD_TEST(PUT)
      METHOD_TEST(POST)
      METHOD_TEST(DELETE)
      METHOD_TEST(TRACE)
      METHOD_TEST(OPTIONS)
      METHOD_TEST(CONNECT)
      METHOD_TEST(PATCH)

      TEST(RequestParser, CheckFields)
      {
        char str[] = "GET / HTTP/1.0\r\n"
          "Host: www.toto.com\r\n"
          "Connection: keep-alive\r\n"
          "Content-Length: 854\r\n"
          "Content-Type: text/*\r\n"
          "Referer: http://tutu.com/hoho%34/?tutu=tata;#anchor\r\n"
          "User-Agent: TomBoy (esapce compatible)\r\n"
          "SomeKey:  SomeValue\r\n"
          "Cookie: attr0; attr01; attr02; attr1=value1; attr2=value2;"
          "        attr3; attr4=value4; attr5=\"xvalue\\o\\\"\"\r\n"
          "Cookie: attr6 = value6 ; attr7 ; attr8 = \" value 8 \" \r\n"
          "\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
        EXPECT_EQ(rq.host(), "www.toto.com");
        EXPECT_EQ(rq.keepAlive(), true);
        EXPECT_EQ(rq.port(), 80);
        EXPECT_EQ(rq.contentLength(), 854);
        EXPECT_EQ(rq.contentType(), "text/*");
        EXPECT_EQ(rq.referrer(), "http://tutu.com/hoho%34/?tutu=tata;#anchor");
        EXPECT_EQ(rq.userAgent(), "TomBoy (esapce compatible)");

        auto it = rq.cookies().find("attr0");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, "");

        it = rq.cookies().find("attr01");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, "");

        it = rq.cookies().find("attr1");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, "value1");

        it = rq.cookies().find("attr2");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, "value2");

        it = rq.cookies().find("attr5");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, std::string("xvalueo\""));

        it = rq.unparsedHeaders().find("SomeKey");
        EXPECT_NE(it, rq.unparsedHeaders().end());
        if (it != rq.unparsedHeaders().end())
          EXPECT_EQ(it->second, "SomeValue");
      }

      TEST(RequestParser, PostHeader)
      {
        const char str[] = "POST /post-echo HTTP/1.1\r\n"
          "Host: localhost:4242\r\n"
          "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:6.0) Gecko/20100101 Firefox/6.0\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en,fr;q=0.5\r\n"
          "Accept-Encoding: gzip, deflate\r\n"
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
          "DNT: 1\r\n"
          "Connection: keep-alive\r\n"
          "Referer: http://localhost:4242/post-echo\r\n"
          "Cache-Control: max-age=0\r\n"
          "Content-Type: application/x-www-form-urlencoded\r\n"
          "Content-Length: 29\r\n"
          "\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
        EXPECT_EQ(rq.port(), 4242);
      }

      TEST(RequestParser, EmptyCookie)
      {
        const char str[] = "GET / HTTP/1.1\r\n"
          "Cookie: key=\r\n"
          "\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
      }

      TEST(RequestParser, 2EmptyCookie)
      {
        const char str[] = "GET / HTTP/1.1\r\n"
          "Cookie: key=; val=\r\n"
          "\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
      }

      TEST(RequestParser, Fail1)
      {
        const char str[] = "POST /login HTTP/1.1\r\n"
          "Host: localhost:19042\r\n"
          "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:9.0.1) Gecko/20100101 Firefox/9.0.1\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en,fr;q=0.5\r\n"
          "Accept-Encoding: gzip, deflate\r\n"
          "Accept-Charset: UTF-8,*\r\n"
          "DNT: 1\r\n"
          "Connection: keep-alive\r\n"
          "Referer: https://localhost:19042/login\r\n"
          "Cookie: login=; auth=\r\n"
          "Cache-Control: max-age=0\r\n"
          "\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
      }

      TEST(RequestParser, Fail2)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Host: localhost:19042\r\n"
          "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:9.0.1) Gecko/20100101 Firefox/9.0.1\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en,fr;q=0.5\r\n"
          "Accept-Encoding: gzip, deflate\r\n"
          "Accept-Charset: UTF-8,*\r\n"
          "DNT: 1\r\n"
          "Connection: keep-alive\r\n"
          "Referer: https://localhost:19042/login\r\n"
          "Cookie: login=; auth=\r\n"
          "Content-Type: application/x-www-form-urlencoded\r\n"
          "Content-Length: 38\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
        ASSERT_EQ(38, rq.contentLength());
      }

      TEST(RequestParser, ContentLength1)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Cookie: login=; auth=\r\n"
          "Content-Length: 38\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
        ASSERT_EQ(38, rq.contentLength());
      }

      TEST(RequestParser, ContentRange_0_499_1234)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Content-Range: bytes 0-499/1234\r\n"
          "\r\n";
        Request rq;
        EXPECT_EQ(true, rq.parse(str, sizeof (str)));
        EXPECT_EQ(true, rq.hasContentRange());
        EXPECT_EQ(0, rq.contentRangeStart());
        EXPECT_EQ(499, rq.contentRangeEnd());
        EXPECT_EQ(1234, rq.contentRangeLength());
      }

      TEST(RequestParser, ContentRange_500_999_1234)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Content-Range: bytes 500-999/1234\r\n"
          "\r\n";
        Request rq;
        EXPECT_EQ(true, rq.parse(str, sizeof (str)));
        EXPECT_EQ(true, rq.hasContentRange());
        EXPECT_EQ(500, rq.contentRangeStart());
        EXPECT_EQ(999, rq.contentRangeEnd());
        EXPECT_EQ(1234, rq.contentRangeLength());
      }

      TEST(RequestParser, ContentRange_500_999_star)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Content-Range: bytes 500-999/*\r\n"
          "\r\n";
        Request rq;
        EXPECT_EQ(true, rq.parse(str, sizeof (str)));
        EXPECT_EQ(true, rq.hasContentRange());
        EXPECT_EQ(500, rq.contentRangeStart());
        EXPECT_EQ(999, rq.contentRangeEnd());
        EXPECT_EQ(-1, rq.contentRangeLength());
      }

      TEST(RequestParser, ContentRange_invalid)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Content-Range: bytes 500-999\r\n"
          "\r\n";
        Request rq;
        EXPECT_EQ(false, rq.parse(str, sizeof (str)));
      }

      TEST(RequestParser, HardCookie1)
      {
        const char str[] =
          "POST /login HTTP/1.1\r\n"
          "Cookie: __utma=172764660.1465859375.1319666592.1320071393.1323170743.5; __utmz=172764660.1320071393.4.4.utmcsr=portail.free.fr|utmccn=(referral)|utmcmd=referral|utmcct=/\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));
      }

      TEST(RequestParser, Cookie1)
      {
        const char str[] =
          "GET /favicon.ico HTTP/1.1\r\n"
          "Host: localhost:19042\r\n"
          "Connection: keep-alive\r\n"
          "Accept: */*\r\n"
          "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.162 Safari/535.19\r\n"
          "Accept-Encoding: gzip,deflate,sdch\r\n"
          "Cookie: login=abique; auth=1fc8f6730ca6349face3c667bb2cb71037cb3e90b85149fbc32963a2d1cd6d72\r\n"
          "Accept-Language: en-US,en;q=0.8\r\n"
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n"
          "\r\n";
        Request rq;
        ASSERT_EQ(true, rq.parse(str, sizeof (str)));

        auto it = rq.cookies().find("login");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, "abique");

        it = rq.cookies().find("auth");
        EXPECT_NE(it, rq.cookies().end());
        if (it != rq.cookies().end())
          EXPECT_EQ(it->second, "1fc8f6730ca6349face3c667bb2cb71037cb3e90b85149fbc32963a2d1cd6d72");
      }
    }
  }
}
