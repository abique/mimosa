#include <gtest/gtest.h>

#include "../../stream/string-stream.hh"
#include "../encoder.hh"

namespace mimosa
{
  namespace json
  {
    namespace tests
    {
      TEST(Encoder, ObjectEmpty)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startObject(), true);
        ASSERT_EQ(enc.endObject(), true);
        ASSERT_EQ(ss->str(), "{}");
      }

      TEST(Encoder, ObjectNull)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startObject(), true);
        ASSERT_EQ(enc.pushString("key1"), true);
        ASSERT_EQ(enc.pushNull(), true);
        ASSERT_EQ(enc.endObject(), true);
        ASSERT_EQ(ss->str(), "{\"key1\":null}");
      }

      TEST(Encoder, ObjectNullNumber)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startObject(), true);
        ASSERT_EQ(enc.pushString("key1"), true);
        ASSERT_EQ(enc.pushNull(), true);
        ASSERT_EQ(enc.pushString("key2"), true);
        ASSERT_EQ(enc.pushNumber(4224), true);
        ASSERT_EQ(enc.endObject(), true);
        ASSERT_EQ(ss->str(), "{\"key1\":null,\"key2\":4224}");
      }

      TEST(Encoder, Infinity)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startArray(), true);
        ASSERT_EQ(enc.pushFloat(std::numeric_limits<double>::infinity()), true);
        ASSERT_EQ(enc.pushFloat(-std::numeric_limits<double>::infinity()), true);
        ASSERT_EQ(enc.endArray(), true);
        ASSERT_EQ(ss->str(), "[\"inf\",\"-inf\"]");
      }

      TEST(Encoder, ObjectBadCloseArray)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startObject(), true);
        ASSERT_THROW(enc.endArray(), SyntaxError);
      }

      TEST(Encoder, ArrayEmpty)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startArray(), true);
        ASSERT_EQ(enc.endArray(), true);
        ASSERT_EQ(ss->str(), "[]");
      }

      TEST(Encoder, ArrayBadCloseObject)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startArray(), true);
        ASSERT_THROW(enc.endObject(), SyntaxError);
      }

      TEST(Encoder, ArrayNull)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startArray(), true);
        ASSERT_EQ(enc.pushNull(), true);
        ASSERT_EQ(enc.endArray(), true);
        ASSERT_EQ(ss->str(), "[null]");
      }

      TEST(Encoder, ArrayString)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startArray(), true);
        ASSERT_EQ(enc.pushString("hello"), true);
        ASSERT_EQ(enc.endArray(), true);
        ASSERT_EQ(ss->str(), "[\"hello\"]");
      }

      TEST(Encoder, ArrayNullStringNumber)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startArray(), true);
        ASSERT_EQ(enc.pushNull(), true);
        ASSERT_EQ(enc.pushString("hello"), true);
        ASSERT_EQ(enc.pushNumber(2442), true);
        ASSERT_EQ(enc.endArray(), true);
        ASSERT_EQ(ss->str(), "[null,\"hello\",2442]");
      }

      TEST(Encoder, True)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushBoolean(true), true);
        ASSERT_EQ(ss->str(), "true");
      }

      TEST(Encoder, False)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushBoolean(false), true);
        ASSERT_EQ(ss->str(), "false");
      }

      TEST(Encoder, Null)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushNull(), true);
        ASSERT_EQ(ss->str(), "null");
      }

      TEST(Encoder, Number0)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushNumber(0), true);
        ASSERT_EQ(ss->str(), "0");
      }

      TEST(Encoder, Number42)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushNumber(42), true);
        ASSERT_EQ(ss->str(), "42");
      }

      TEST(Encoder, NumberNeg42)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushNumber(-42), true);
        ASSERT_EQ(ss->str(), "-42");
      }

      TEST(Encoder, StringEmpty)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushString(""), true);
        ASSERT_EQ(ss->str(), "\"\"");
      }

      TEST(Encoder, StringSimple)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushString("simple"), true);
        ASSERT_EQ(ss->str(), "\"simple\"");
      }

      TEST(Encoder, StringEscape)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.pushString("s\"imp\\le"), true);
        ASSERT_EQ(ss->str(), "\"s\\\"imp\\\\le\"");
      }

      TEST(Encoder, Big)
      {
        auto ss = new stream::StringStream;
        Encoder enc(ss);

        ASSERT_EQ(enc.startObject(), true);
        ASSERT_EQ(enc.pushString("result"), true);
        ASSERT_EQ(enc.startArray(), true);
        ASSERT_EQ(enc.startObject(), true);
        ASSERT_EQ(enc.pushString("path"), true);
        ASSERT_EQ(enc.pushString("/run/titi"), true);
        ASSERT_EQ(enc.pushString("id"), true);
        ASSERT_EQ(enc.pushNumber(42), true);
        ASSERT_EQ(enc.endObject(), true);
        ASSERT_EQ(enc.endArray(), true);
        ASSERT_EQ(enc.endObject(), true);
        ASSERT_EQ(ss->str(), "{\"result\":[{\"path\":\"/run/titi\",\"id\":42}]}");
      }
    }
  }
}
