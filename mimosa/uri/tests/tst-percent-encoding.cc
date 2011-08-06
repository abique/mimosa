#include <gtest/gtest.h>

#include "../percent-encoding.hh"

namespace mimosa
{
  namespace uri
  {
    namespace
    {
      TEST(PercentEncoding, Simple)
      {
#define M(Input, Ref2396, Ref3986)                                      \
        do {                                                            \
          std::string output;                                           \
          percentEncode(Input, sizeof (Input) - 1, &output, kRfc2396);  \
          EXPECT_EQ(output, Ref2396);                                   \
          percentDecode(Ref2396, sizeof (Ref2396) - 1, &output, kRfc2396); \
          EXPECT_EQ(Input, output);                                     \
          percentEncode(Input, sizeof (Input) - 1, &output, kRfc3986);  \
          EXPECT_EQ(output, Ref3986);                                   \
          percentDecode(Ref3986, sizeof (Ref3986) - 1, &output, kRfc3986); \
          EXPECT_EQ(Input, output);                                     \
        } while (0)

        M("b", "b", "b");
        M(" ", "+", "%20");
        M("+", "%2B", "%2B");
        M("!*'();:@&;$,/?#[]", "!*'();:@&;$,/?#[]", "!*'();:@&;$,/?#[]");

#undef M
      }
    }
  }
}
