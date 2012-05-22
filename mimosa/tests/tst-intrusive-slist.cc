#include <gtest/gtest.h>

#include "../intrusive-slist.hh"
#include "../../ref-countable.hh"

namespace mimosa
{
  namespace container
  {
    namespace
    {
      struct A : public RefCountable<A>
      {
        inline A(int i) : i_(i) {}

        int i_;
        IntrusiveSlistHook<A::Ptr> hook_;
      };

      typedef IntrusiveSlist<A, A::Ptr, &A::hook_> slist_type;

      namespace
      {
        TEST(IntrusiveSlist, Simple)
        {
          slist_type list;

          ASSERT_EQ(list.size(), 0);
          ASSERT_EQ(list.empty(), true);

          auto a1 = new A(1);
          list.push(a1);

          ASSERT_EQ(list.size(), 1);
          ASSERT_EQ(list.empty(), false);
          ASSERT_EQ(list.front()->i_, 1);
          ASSERT_EQ(list.back()->i_, 1);
          ASSERT_EQ(list.front()->hook_.next_, a1);

          {
            auto it = list.begin();
            ASSERT_NE(it, list.end());
            ASSERT_EQ(it->i_, 1);

            ++it;
            ASSERT_EQ(it, list.end());
          }

          auto a2 = new A(2);
          list.push(a2);

          ASSERT_EQ(list.size(), 2);
          ASSERT_EQ(list.empty(), false);
          ASSERT_EQ(list.front()->i_, 1);
          ASSERT_EQ(list.front()->hook_.next_, a2);
          ASSERT_EQ(list.back()->hook_.next_, a1);
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
          list.push(a3);

          ASSERT_EQ(list.size(), 3);
          ASSERT_EQ(list.empty(), false);
          ASSERT_EQ(list.front()->i_, 1);
          ASSERT_EQ(list.front()->hook_.next_, a2);
          ASSERT_EQ(list.back()->hook_.next_, a1);
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

          list.pop();

          ASSERT_EQ(list.size(), 2);
          ASSERT_EQ(list.empty(), false);
          ASSERT_EQ(list.front()->i_, 2);
          ASSERT_EQ(list.front()->hook_.next_, a3);
          ASSERT_EQ(list.back()->hook_.next_, a2);
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

          list.pop();

          ASSERT_EQ(list.size(), 1);
          ASSERT_EQ(list.empty(), false);

          list.pop();

          ASSERT_EQ(list.size(), 0);
          ASSERT_EQ(list.empty(), true);
        }
      }
    }
  }
}
