#include <gtest/gtest.h>

#include "../normalize-path.hh"

namespace mimosa
{
  namespace uri
  {
    namespace
    {
      TEST(NormalizePath, Simple)
      {
#define M(Input, Ref)                                           \
        do {                                                    \
          std::string output;                                   \
          normalizePath(Input, sizeof (Input) - 1, &output);    \
          EXPECT_EQ(output, Ref);                               \
        } while (0)

        M("", ".");
        M(".", ".");
        M("./", ".");
        M("./toto", "toto");
        M("toto", "toto");
        M("toto/", "toto/");
        M("/.", "/");
        M("/", "/");
        M("/toto", "/toto");
        M("//", "/");
        M("/a/b/c", "/a/b/c");
        M("/a/b/c/", "/a/b/c/");
        M("/a/b/c/", "/a/b/c/");
        M("/a/b///c/", "/a/b/c/");
        M("/a/b/./c/", "/a/b/c/");

        M("/././././../../.././", "/");
        M("/././././../../.././b", "/b");
        M("/a/b/c/../../", "/a/");
        M("/a/b/c/../..", "/a/");
        M("/a/b/c/..", "/a/b/");
        M("/a/b/c/../d", "/a/b/d");
        M("/a/b/c/../d/", "/a/b/d/");
        M("/a/b/c/.././d/", "/a/b/d/");
        M("/a/b/c/../d/.", "/a/b/d/");
#undef M
      }
    }
  }
}
