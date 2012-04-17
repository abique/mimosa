#include <gtest/gtest.h>

#include "../response.hh"

namespace mimosa
{
  namespace http
  {
    namespace
    {
      TEST(Response, Cookie)
      {
        Response resp;

        resp.status_ = kStatusOk;
        resp.keep_alive_ = true;
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

        EXPECT_EQ(
          str,
          "HTTP/1.1 200 OK\r\n"
          "Server: mimosa\r\n"
          "Connection: Keep-Alive\r\n"
          "Content-Length: 42\r\n"
          "Content-Type: text/html\r\n"
          "Transfer-Encoding: Identity\r\n"
          "Set-Cookie: key1=value1; Secure; HttpOnly\r\n"
          "Set-Cookie: key2=value2; Domain=toto.com; Path=/titi\r\n"
          "\r\n");
      }
    }
  }
}
