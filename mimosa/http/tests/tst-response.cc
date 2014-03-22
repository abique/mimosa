#include <gtest/gtest.h>

#include "../response.hh"

namespace mimosa
{
  namespace http
  {
    namespace
    {
#if 0
      TEST(Response, Cookie)
      {
        Response resp;

        resp.setStatus(kStatusOk);
        resp.setKeepAlive(true);
        resp.content_encoding_ = kCodingIdentity;
        resp.transfer_encoding_ = kCodingIdentity;
        resp.content_length_ = 42;
        resp.content_type_ = "text/html";

        auto cookie = new Cookie;
        cookie->setKey("key1");
        cookie->setValue("value1");
        cookie->setHttpOnly(true);
        cookie->setSecure(true);
        resp.cookies_.push(cookie);

        cookie = new Cookie;
        cookie->setKey("key2");
        cookie->setValue("value2");
        cookie->setDomain("toto.com");
        cookie->setPath("/titi");
        cookie->setHttpOnly(false);
        cookie->setSecure(false);
        resp.cookies_.push(cookie);

        auto str = resp.toHttpHeader();
        static const char *ref =
          "HTTP/1.1 200 OK\r\n"
          "Server: mimosa\r\n"
          "Connection: Keep-Alive\r\n"
          "Content-Length: 42\r\n"
          "Content-Type: text/html\r\n"
          //"Transfer-Encoding: Identity\r\n"
          "Set-Cookie: key1=value1; Secure; HttpOnly\r\n"
          "Set-Cookie: key2=value2; Domain=toto.com; Path=/titi\r\n"
          "\r\n";

        EXPECT_EQ(ref, str);
      }

      TEST(Response, ContentRange)
      {
        Response resp;

        resp.status_ = kStatusPartialContent;
        resp.setContentRange(1, 2, 3);

        auto str = resp.toHttpHeader();
        static const char *ref =
          "HTTP/1.1 206 PartialContent\r\n"
          "Server: mimosa\r\n"
          "Connection: Close\r\n"
          "Content-Range: bytes 1-2/3\r\n"
          "Content-Type: text/plain\r\n"
          "\r\n";

        EXPECT_EQ(ref, str);
      }
#endif

      TEST(Response, Parse1)
      {
        const char str[] =
          "HTTP/1.1 200 OK\r\n"
          "\r\n";
        Response r;
        EXPECT_EQ(true, r.parse(str, sizeof (str)));
        EXPECT_EQ(r.status(), 200);
      }

      TEST(Response, Parse2)
      {
        const char str[] =
          "HTTP/1.1 200 OK\r\n"
          "Server: nginx\r\n"
          "Date: Sat, 22 Mar 2014 14:30:42 GMT\r\n"
          "Content-Type: text/html; charset=UTF-8\r\n"
          "Transfer-Encoding: chunked\r\n"
          "Connection: keep-alive\r\n"
          "Vary: Accept-Encoding\r\n"
          "Vary: Cookie\r\n"
          "X-Pingback: http://www.ektoplazm.com/xmlrpc.php\r\n"
          "WP-Super-Cache: Served legacy cache file\r\n"
          "Access-Control-Allow-Origin: *\r\n"
          "Cache-Control: no-transform\r\n"
          "X-Frame-Options: sameorigin\r\n"
          "X-UA-Compatible: IE=Edge,chrome=1\r\n"
          "\r\n";
        Response r;
        EXPECT_EQ(true, r.parse(str, sizeof (str)));
        EXPECT_EQ(r.status(), 200);
        EXPECT_EQ(r.contentType(), "text/html; charset=UTF-8");
        EXPECT_EQ(r.transferEncoding(), kCodingChunked);
      }
    }
  }
}
