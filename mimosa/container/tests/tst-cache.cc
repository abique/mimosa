#include <gtest/gtest.h>

#include "../cache.hh"
#include "../cache.hxx"

namespace mimosa
{
  namespace container
  {
    namespace
    {
      class MyCache : public Cache<int, int>
      {
      protected:
        virtual void cacheMiss(const int & key)
        {
          set(key, key + 42);
        }
      };

      TEST(Cache, Simple)
      {
        MyCache cache;

        auto val = cache.get(0);
        ASSERT_EQ(val->get(), 42);

        val = cache.get(0);
        ASSERT_EQ(val->get(), 42);
      }
    }
  }
}
