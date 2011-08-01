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
      METHOD_TEST(DELETE)
      METHOD_TEST(TRACE)
      METHOD_TEST(OPTIONS)
      METHOD_TEST(CONNECT)
      METHOD_TEST(PATCH)

      TEST(RequestParser, CheckFields)
      {
        // ASSERT_NE(std::hash<std::string>(std::string("attr0")),
        //           std::hash<std::string>(std::string("attr1")));

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
        EXPECT_EQ(true, rq.parse(str, sizeof (str)));
        EXPECT_EQ(rq.host_, "www.toto.com");
        EXPECT_EQ(rq.keep_alive_, true);
        EXPECT_EQ(rq.content_length_, 854);
        EXPECT_EQ(rq.content_type_, "text/*");
        EXPECT_EQ(rq.referrer_, "http://tutu.com/hoho%34/?tutu=tata;#anchor");
        EXPECT_EQ(rq.user_agent_, "TomBoy (esapce compatible)");

        auto it = rq.cookies_.find("attr1");
        EXPECT_NE(it, rq.cookies_.end());
        if (it != rq.cookies_.end())
          EXPECT_EQ(it->second, "value1");
        it = rq.cookies_.find("attr5");
        EXPECT_NE(it, rq.cookies_.end());
        if (it != rq.cookies_.end())
          EXPECT_EQ(it->second, std::string("xvalueo\""));

        it = rq.unparsed_headers_.find("SomeKey");
        EXPECT_NE(it, rq.unparsed_headers_.end());
        if (it != rq.unparsed_headers_.end())
          EXPECT_EQ(it->second, "SomeValue");
      }
    }
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
