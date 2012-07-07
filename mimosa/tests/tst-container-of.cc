#include <gtest/gtest.h>

#include "../container-of.hh"

namespace mimosa
{
  namespace tests
  {
    namespace
    {
      struct A
      {
        int dummy;
      };

      struct B
      {
        A    bibi;
        char tutu[128];
        A    tata;
        char toto[64];
        A    tetons;
      };

      TEST(ContainerOf, Simple)
      {
        B   b;
        A * a;
        B * b2;

        a = &b.bibi;
        b2 = containerOf(a, &B::bibi);
        EXPECT_EQ(&b, b2);

        a = &b.tata;
        b2 = containerOf(a, &B::tata);
        EXPECT_EQ(&b, b2);

        a = &b.tetons;
        b2 = containerOf(a, &B::tetons);
        EXPECT_EQ(&b, b2);
      }
    }
  }
}
