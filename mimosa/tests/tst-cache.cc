#include <gtest/gtest.h>

#include "../expiring-cache.hh"
#include "../expiring-cache.hxx"

namespace mimosa
{
  namespace
  {
    class MyCache : public ExpiringCache<int, int>
    {
    protected:
      virtual void cacheMiss(const int & key)
      {
        set(key, key + 42);
      }
    };

    TEST(ExpiringCache, Simple)
    {
      MyCache cache;

      auto val = cache.get(0);
      ASSERT_EQ(val->get(), 42);

      val = cache.get(0);
      ASSERT_EQ(val->get(), 42);
    }
  }
}
