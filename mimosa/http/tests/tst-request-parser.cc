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
    }
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
