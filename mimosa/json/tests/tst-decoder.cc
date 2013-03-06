#include <gtest/gtest.h>

#include "../../stream/string-stream.hh"
#include "../decoder.hh"

namespace mimosa
{
  namespace json
  {
    namespace tests
    {
      TEST(Decoder, Null)
      {
        auto ss = new stream::StringStream("null");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kNull, dec.pull());
      }

      TEST(Decoder, True)
      {
        auto ss = new stream::StringStream("true");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kBoolean, dec.pull());
        ASSERT_EQ(true, dec.boolean());
      }

      TEST(Decoder, False)
      {
        auto ss = new stream::StringStream("false");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kBoolean, dec.pull());
        ASSERT_EQ(false, dec.boolean());
      }

#define TEST_STRING(Name, Str, Value)                   \
                                                        \
      TEST(Decoder, String##Name)                       \
      {                                                 \
        auto ss = new stream::StringStream(Str);        \
        Decoder dec(ss);                                \
                                                        \
        ASSERT_EQ(Decoder::kString, dec.pull());        \
        ASSERT_EQ(Value, dec.string());                 \
      }

      TEST_STRING(Hello, "\"hello\"", "hello")
      TEST_STRING(HeTabllo, "\"he\\tllo\"", "he\tllo")
      TEST_STRING(HeCrLfllo, "\"he\\r\\nllo\"", "he\r\nllo")

#define TEST_INTEGER(Name, String, Value)               \
                                                        \
      TEST(Decoder, Integer##Name)                      \
      {                                                 \
        auto ss = new stream::StringStream(String);     \
        Decoder dec(ss);                                \
                                                        \
        ASSERT_EQ(Decoder::kInteger, dec.pull());       \
        ASSERT_EQ(Value, dec.integer());                \
      }

      TEST_INTEGER(0, "0", 0)
      TEST_INTEGER(Neg0, "-0", 0)
      TEST_INTEGER(1, "1", 1)
      TEST_INTEGER(Neg1, "-1", -1)
      TEST_INTEGER(Neg42, "-42", -42)
      TEST_INTEGER(Neg684932, "684932", 684932)
      TEST_INTEGER(Neg00684932, "00684932", 684932)

#define TEST_RATIONAL(Name, String, Value)              \
                                                        \
      TEST(Decoder, Rational##Name)                     \
      {                                                 \
        auto ss = new stream::StringStream(String);     \
        Decoder dec(ss);                                \
                                                        \
        ASSERT_EQ(Decoder::kRational, dec.pull());      \
        ASSERT_DOUBLE_EQ(Value, dec.rational());        \
      }

      TEST_RATIONAL(0, "0.0", 0)
      TEST_RATIONAL(0p1, "0.1", 0.1)
      TEST_RATIONAL(0p9, "0.9", 0.9)
      TEST_RATIONAL(0p01, "0.01", 0.01)
      TEST_RATIONAL(Neg0p01, "-0.01", -0.01)
      TEST_RATIONAL(6p01, "6.01", 6.01)
      TEST_RATIONAL(6e3, "6e3", 6000)
      TEST_RATIONAL(Neg6e3, "-6e3", -6000)
      TEST_RATIONAL(6d40143e3, "6.40143e3", 6401.43)
      TEST_RATIONAL(6E3, "6E3", 6000)
      TEST_RATIONAL(NEg6E3, "-6E3", -6000)
      TEST_RATIONAL(6d40143E3, "6.40143E3", 6401.43)

      TEST(Decoder, ArrayEmpty)
      {
        auto ss = new stream::StringStream("[]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, ArrayArray)
      {
        auto ss = new stream::StringStream("[[]]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, ArrayArrayArray)
      {
        auto ss = new stream::StringStream("[[[]]]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, ArrayArrayArrayArray)
      {
        auto ss = new stream::StringStream("[[[[]]]]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, ArrayRepeat)
      {
        auto ss = new stream::StringStream("[[],[1,2],[]]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(1, dec.integer());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(2, dec.integer());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, ArrayIntInt)
      {
        auto ss = new stream::StringStream("[32,33]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(32, dec.integer());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(33, dec.integer());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, ObjectEmpty)
      {
        auto ss = new stream::StringStream("{}");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kObjectBegin, dec.pull());
        ASSERT_EQ(Decoder::kObjectEnd, dec.pull());
      }

      TEST(Decoder, Object1)
      {
        auto ss = new stream::StringStream("{\"key1\":42}");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kObjectBegin, dec.pull());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("key1", dec.string());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(42, dec.integer());
        ASSERT_EQ(Decoder::kObjectEnd, dec.pull());
      }

      TEST(Decoder, Object2)
      {
        auto ss = new stream::StringStream("{\"key\":\"tutu\",\"value\":\"tata\"}");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kObjectBegin, dec.pull());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("key", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("tutu", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("value", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("tata", dec.string());
        ASSERT_EQ(Decoder::kObjectEnd, dec.pull());
      }

      TEST(Decoder, Object3)
      {
        auto ss = new stream::StringStream("{\"key\":\"tutu\",\"value\":\"tata\",\"opt\":\"toptop\"}");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kObjectBegin, dec.pull());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("key", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("tutu", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("value", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("tata", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("opt", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("toptop", dec.string());
        ASSERT_EQ(Decoder::kObjectEnd, dec.pull());
      }

      TEST(Decoder, EatValue1)
      {
        auto ss = new stream::StringStream("[21,32,45]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(21, dec.integer());
        dec.eatValue();
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(45, dec.integer());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

      TEST(Decoder, EatValue2)
      {
        auto ss = new stream::StringStream("{\"key\":\"tutu\",\"value\":\"tata\"}");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kObjectBegin, dec.pull());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("key", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("tutu", dec.string());
        ASSERT_EQ(Decoder::kString, dec.pull());
        ASSERT_EQ("value", dec.string());
        dec.eatValue();
        ASSERT_EQ(Decoder::kObjectEnd, dec.pull());
      }

      TEST(Decoder, EatValue3)
      {
        auto ss = new stream::StringStream("[21,{\"key\":\"tutu\",\"value\":\"tata\"},45]");
        Decoder dec(ss);

        ASSERT_EQ(Decoder::kArrayBegin, dec.pull());
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(21, dec.integer());
        dec.eatValue();
        ASSERT_EQ(Decoder::kInteger, dec.pull());
        ASSERT_EQ(45, dec.integer());
        ASSERT_EQ(Decoder::kArrayEnd, dec.pull());
      }

#define TEST_THROW(Name, Exception, String, Nb)         \
                                                        \
      TEST(Decoder, Exception##Name)                    \
      {                                                 \
        auto ss = new stream::StringStream(String);     \
        Decoder dec(ss);                                \
                                                        \
        for (int i = 0; i < Nb; ++i)                    \
          ASSERT_NE(Decoder::kEof, dec.pull());         \
        ASSERT_THROW(dec.pull(), Decoder::Exception);   \
      }

      TEST_THROW(IntegerNegNeg, SyntaxError, "--", 0)
      TEST_THROW(Rational0dd1, SyntaxError, "0..1", 0)
      TEST_THROW(Rational0d1d2, SyntaxError, "0.1.2", 1)
      TEST_THROW(Rational0e1d2, SyntaxError, "0e1.2", 1)
      TEST_THROW(nll, PrematureEof, "nll", 0)
      TEST_THROW(nall, SyntaxError, "nall", 0)
      TEST_THROW(Null, SyntaxError, "Null", 0)
      TEST_THROW(tue, PrematureEof, "tue", 0)
      TEST_THROW(tdue, SyntaxError, "tdue", 0)
      TEST_THROW(True, SyntaxError, "True", 0)
      TEST_THROW(flse, PrematureEof, "flse", 0)
      TEST_THROW(fulse, SyntaxError, "fulse", 0)
      TEST_THROW(False, SyntaxError, "False", 0)
      TEST_THROW(Array1, SyntaxError, "[,]", 1)
      TEST_THROW(Array2, SyntaxError, "[32\"hello\"]", 2)
      TEST_THROW(Array3, SyntaxError, "[32,]", 2)
      TEST_THROW(Array4, SyntaxError, "[32,42,]", 3)
      TEST_THROW(Array5, SyntaxError, "[[32,42],]", 5)
      TEST_THROW(Object1, SyntaxError, "{42", 1)
      TEST_THROW(Object2, SyntaxError, "{{", 1)
      TEST_THROW(Object3, SyntaxError, "{[", 1)
      TEST_THROW(Object4, SyntaxError, "{\"key\",32}", 2)
      TEST_THROW(Object4, PrematureEof, "{\"key\":32", 3)
    }
  }
}
