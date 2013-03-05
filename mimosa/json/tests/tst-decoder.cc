#include <gtest/gtest.h>

#include "../../stream/string-stream.hh"
#include "../decoder.hh"

namespace mimosa
{
  namespace json
  {
    namespace tests
    {
#define TEST_INTEGER(Name, String, Value)               \
                                                        \
      TEST(Decoder, Integer##Name)                      \
      {                                                 \
        auto ss = new stream::StringStream(String);     \
        Decoder dec(ss);                                \
                                                        \
        ASSERT_EQ(dec.pull(), Decoder::kInteger);       \
        ASSERT_EQ(Value, dec.integer());                \
      }

#define TEST_RATIONAL(Name, String, Value)              \
                                                        \
      TEST(Decoder, Rational##Name)                     \
      {                                                 \
        auto ss = new stream::StringStream(String);     \
        Decoder dec(ss);                                \
                                                        \
        ASSERT_EQ(dec.pull(), Decoder::kRational);      \
        ASSERT_DOUBLE_EQ(Value, dec.rational());        \
      }

      TEST_INTEGER(0, "0", 0)
      TEST_INTEGER(Neg0, "-0", 0)
      TEST_INTEGER(1, "1", 1)
      TEST_INTEGER(Neg1, "-1", -1)
      TEST_INTEGER(Neg42, "-42", -42)
      TEST_INTEGER(Neg684932, "684932", 684932)
      TEST_INTEGER(Neg00684932, "00684932", 684932)

      TEST_RATIONAL(0, "0.0", 0)
      TEST_RATIONAL(0p1, "0.1", 0.1)
      TEST_RATIONAL(0p9, "0.9", 0.9)
      TEST_RATIONAL(0p01, "0.01", 0.01)
      TEST_RATIONAL(Neg0p01, "-0.01", -0.01)
      TEST_RATIONAL(6p01, "6.01", 6.01)
      TEST_RATIONAL(6e3, "6e3", 6000)
      TEST_RATIONAL(Neg6e3, "-6e3", -6000)
      TEST_RATIONAL(6d40143e3, "6.40143e3", 6401.43)
    }
  }
}
