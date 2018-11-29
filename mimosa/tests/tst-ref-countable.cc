#include <gtest/gtest.h>

#include "../ref-countable.hh"

namespace mimosa
{
  namespace
  {
    struct A : public RefCountable<A>
    {
      int v;
    };

    TEST(RefCountable, Simple)
    {
      A::Ptr obj = new A;
      obj = nullptr;
    }

    TEST(RefCountable, SimplePtr)
    {
      A::Ptr obj = new A;
      A::Ptr obj2(obj.get());
    }

    TEST(RefCountable, Assign)
    {
      A::Ptr obj = new A;
      A::Ptr obj2;

      obj2 = nullptr;
      obj2 = obj;
      obj2 = nullptr;
      obj2 = obj;
    }

    TEST(RefCountable, CopyConstructor)
    {
      A::Ptr obj = new A;
      const A::Ptr& obj2(obj);
    }
  }
}
