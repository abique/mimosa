#include <gtest/gtest.h>

#include "../intrusive-dlist.hh"
#include "../ref-countable.hh"

namespace mimosa
{
  namespace
  {
    struct A : public RefCountable<A>
    {
      inline A(int i) : i_(i) {}

      int i_;
      IntrusiveDListHook<A::Ptr> hook_;
    };

    typedef IntrusiveDList<A, A::Ptr, &A::hook_> dlist_type;

    namespace
    {
      TEST(IntrusiveDList, Simple)
      {
        dlist_type list;

        ASSERT_EQ(list.size(), 0);
        ASSERT_EQ(list.empty(), true);

        auto a1 = new A(1);
        list.pushBack(a1);

        ASSERT_EQ(list.size(), 1);
        ASSERT_EQ(list.empty(), false);
        ASSERT_EQ(list.front()->i_, 1);
        ASSERT_EQ(list.back()->i_, 1);

        {
          auto it = list.begin();
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 1);

          ++it;
          ASSERT_EQ(it, list.end());
        }

        auto a2 = new A(2);
        list.pushBack(a2);

        ASSERT_EQ(list.size(), 2);
        ASSERT_EQ(list.empty(), false);
        ASSERT_EQ(list.front()->i_, 1);
        ASSERT_EQ(list.back()->i_, 2);

        {
          auto it = list.begin();
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 1);

          ++it;
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 2);

          ++it;
          ASSERT_EQ(it, list.end());
        }

        auto a3 = new A(3);
        list.pushBack(a3);

        ASSERT_EQ(list.size(), 3);
        ASSERT_EQ(list.empty(), false);
        ASSERT_EQ(list.front()->i_, 1);
        ASSERT_EQ(list.back()->i_, 3);

        {
          auto it = list.begin();
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 1);

          ++it;
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 2);

          ++it;
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 3);

          ++it;
          ASSERT_EQ(it, list.end());
        }

        list.popFront();

        ASSERT_EQ(list.size(), 2);
        ASSERT_EQ(list.empty(), false);
        ASSERT_EQ(list.front()->i_, 2);
        ASSERT_EQ(list.back()->i_, 3);

        {
          auto it = list.begin();
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 2);

          ++it;
          ASSERT_NE(it, list.end());
          ASSERT_EQ(it->i_, 3);

          ++it;
          ASSERT_EQ(it, list.end());
        }

        list.popFront();

        ASSERT_EQ(list.size(), 1);
        ASSERT_EQ(list.empty(), false);

        list.popFront();

        ASSERT_EQ(list.size(), 0);
        ASSERT_EQ(list.empty(), true);
      }
    }
  }
}
