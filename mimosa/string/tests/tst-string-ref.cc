#include <gtest/gtest.h>

#include "../string-ref.hh"

namespace mimosa
{
  namespace string
  {
    namespace
    {
      TEST(StringRef, tokens1)
      {
        StringRef s("abc");
        auto vec = s.tokens('.');

        ASSERT_EQ(vec.size(), 1);
        ASSERT_EQ(vec[0], "abc");
      }

      TEST(StringRef, tokens2)
      {
        StringRef s("ab.c");
        auto vec = s.tokens('.');

        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec[0], "ab");
        ASSERT_EQ(vec[1], "c");
      }

      TEST(StringRef, tokens3)
      {
        StringRef s("ab.c.d.");
        auto vec = s.tokens('.');

        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec[0], "ab");
        ASSERT_EQ(vec[1], "c");
        ASSERT_EQ(vec[2], "d");
      }

      TEST(StringRef, tokens4)
      {
        StringRef s("...ab.c.d.");
        auto vec = s.tokens('.');

        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec[0], "ab");
        ASSERT_EQ(vec[1], "c");
        ASSERT_EQ(vec[2], "d");
      }

      TEST(StringRef, tokens5)
      {
        StringRef s("...");
        auto vec = s.tokens('.');

        ASSERT_EQ(vec.size(), 0);
      }
    }
  }
}
