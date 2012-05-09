#include <gtest/gtest.h>

#include "../../ref-countable.hh"
#include "../trie.hh"

namespace mimosa
{
  namespace container
  {
    namespace
    {
      struct Value : public RefCountable<Value>
      {
        Value(const char *str) : data_(str) {}

        const char *data_;
      };

      string::StringRef getKey(Value::Ptr value)
      {
        return string::StringRef(value->data_);
      }

      TEST(Trie, Simple)
      {
        Trie<Value::Ptr, getKey> trie;

        Value::Ptr ptr(new Value("bonjour"));
        auto ret = trie.insert(ptr);
        EXPECT_EQ(ret, true);

        ret = trie.insert(ptr);
        EXPECT_EQ(ret, false);

        ptr = new Value("bonjoun");
        ret = trie.insert(ptr);
        EXPECT_EQ(ret, true);

        EXPECT_EQ(trie.empty(), false);
        EXPECT_EQ(trie.size(), 2);

        ptr = trie.find("suce");
        EXPECT_EQ(ptr, nullptr);

        ptr = trie.find("bonjour");
        EXPECT_NE(ptr, nullptr);
        if (ptr)
          EXPECT_EQ(ptr->data_, "bonjour");

        trie.erase("bonjoun");

        EXPECT_EQ(trie.empty(), false);
        EXPECT_EQ(trie.size(), 1);

        trie.erase("bonjour");

        EXPECT_EQ(trie.empty(), true);
        EXPECT_EQ(trie.size(), 0);
      }
    }
  }
}
