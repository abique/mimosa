#include <gtest/gtest.h>

#include "../string-stream.hh"
#include "../base16-encoder.hh"
#include "../base16-decoder.hh"

namespace mimosa
{
  namespace stream
  {
    namespace
    {
#define FILTER_TEST(Type, Input, Output)                        \
      TEST(Hash##Type, Simple)                                  \
      {                                                         \
        StringStream::Ptr str = new StringStream;               \
        Type::Ptr filter = new Type(str.get());                 \
        filter->write(Input, sizeof (Input) - 1);               \
        ASSERT_EQ(str->str(), Output);                          \
      }

      FILTER_TEST(Base16Encoder, "bonjour", "626f6e6a6f7572")
      FILTER_TEST(Base16Decoder, "626f6e6a6f7572", "bonjour")
    }
  }
}
