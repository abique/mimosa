#include <gtest/gtest.h>

#include "../format.hh"

namespace mimosa
{
  namespace format
  {
    namespace
    {
      TEST(Format, Simple)
      {
        EXPECT_EQ("1", str("1"));
        EXPECT_EQ("42", str("%v", 42));
        EXPECT_EQ("42", str("%", 42));
        EXPECT_EQ("42", str("%d", 42));
        EXPECT_EQ("2a", str("%x", 42));
        EXPECT_EQ("2a", str("%x", 42));
        EXPECT_EQ("%", str("%%", 42));
      }
    }
  }
}
