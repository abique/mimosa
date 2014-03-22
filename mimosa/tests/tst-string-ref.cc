#include <gtest/gtest.h>

#include "../string-ref.hh"

namespace mimosa
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

    TEST(StringRef, Eq)
    {
      StringRef s1("abc");
      StringRef s2("abc");

      EXPECT_EQ(s1, s2);
    }

    TEST(StringRef, Neq)
    {
      StringRef s1("abc");
      StringRef s2("abd");

      EXPECT_NE(s1, s2);
    }

    TEST(StringRef, Atoi)
    {
      EXPECT_EQ(StringRef("24").atoi<int>(), 24);
      EXPECT_EQ(StringRef("-5").atoi<int>(), -5);
    }

    TEST(StringRef, AtoiHex)
    {
      EXPECT_EQ(StringRef("0").atoiHex<int>(), 0);
      EXPECT_EQ(StringRef("-5").atoiHex<int>(), -5);
      EXPECT_EQ(StringRef("b").atoiHex<int>(), 11);
      EXPECT_EQ(StringRef("10").atoiHex<int>(), 16);
      EXPECT_EQ(StringRef("1F").atoiHex<int>(), 31);
    }

    TEST(StringRef, ConsumeToken)
    {
      StringRef s1("tutu \t");
      StringRef s2(s1.consumeToken(" \t\v\r"));

      EXPECT_EQ(s1, " \t");
      EXPECT_EQ(s2, "tutu");
    }

    TEST(StringRef, strncaseeq)
    {
      StringRef s1("http/1.1");
      EXPECT_TRUE(s1.strncaseeq("http/1."));
    }

    TEST(StringRef, eatWhitespaces)
    {
      StringRef s1("  \ttatata");
      s1.eatWhitespaces(" \t");
      EXPECT_EQ(s1, StringRef("tatata"));
    }
  }
}
