#include <gtest/gtest.h>

#include "../string-stream.hh"
#include "../base16-encoder.hh"
#include "../base16-decoder.hh"
#include "../base64-decoder.hh"

namespace mimosa
{
  namespace stream
  {
    namespace
    {
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define FILTER_TEST(Type, Input, Output)                        \
      TEST(Type, TOKENPASTE2(Simple, __LINE__))                 \
      {                                                         \
        StringStream::Ptr str = new StringStream;               \
        Type::Ptr filter = new Type(str.get());                 \
        filter->write(Input, sizeof (Input) - 1);               \
        ASSERT_EQ(str->str(), Output);                          \
      }

      FILTER_TEST(Base16Encoder, "bonjour", "626f6e6a6f7572")
      FILTER_TEST(Base16Decoder, "626f6e6a6f7572", "bonjour")

      FILTER_TEST(Base64Decoder, "cGxlYXN1cmUu", "pleasure.")
      FILTER_TEST(Base64Decoder, "bGVhc3VyZS4=", "leasure.")
      FILTER_TEST(Base64Decoder, "ZWFzdXJlLg==", "easure.")
      FILTER_TEST(Base64Decoder, "YXN1cmUu", "asure.")
      FILTER_TEST(Base64Decoder, "c3VyZS4=", "sure.")
    }
  }
}
