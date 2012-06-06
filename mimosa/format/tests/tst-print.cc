#include <gtest/gtest.h>

#include "../print.hh"
#include "../../stream/string-stream.hh"

#define CHECK_BYTE_SIZE(Size, Ref)              \
  do {                                          \
    stream::StringStream s;                     \
    printByteSize(s, Size);                     \
    EXPECT_EQ(Ref, s.str());                    \
  } while (0)

namespace mimosa
{
  namespace format
  {
    namespace tests
    {
      TEST(Print, ByteSize)
      {
        CHECK_BYTE_SIZE(0, "0B");

        CHECK_BYTE_SIZE(1ull << 0, "1B");
        CHECK_BYTE_SIZE(1ull << 10, "1KiB");
        CHECK_BYTE_SIZE(1ull << 20, "1MiB");
        CHECK_BYTE_SIZE(1ull << 30, "1GiB");
        CHECK_BYTE_SIZE(1ull << 40, "1TiB");
        CHECK_BYTE_SIZE(1ull << 50, "1PiB");
        CHECK_BYTE_SIZE(1ull << 60, "1EiB");

        CHECK_BYTE_SIZE(1023, "1023B");

        CHECK_BYTE_SIZE(2*1024, "2KiB");
        CHECK_BYTE_SIZE(2*1024 + 512, "2.5KiB");
        CHECK_BYTE_SIZE(2*1024 + 512 + 256, "2.75KiB");
        CHECK_BYTE_SIZE(20*1024 + 512 + 256, "20.7KiB");
        CHECK_BYTE_SIZE(200*1024 + 512 + 256, "200KiB");
        CHECK_BYTE_SIZE(2*1024*1024 + 512, "2MiB");
      }
    }
  }
}
