#include <gtest/gtest.h>

#include "../ref-countable.hh"
#include "../trie.hh"

# define FIND(Key, Success)                     \
  do {                                          \
    Value::Ptr item = trie.find(Key);           \
    if (Success)                                \
    {                                           \
      EXPECT_NE(nullptr, item);                 \
      if (item)                                 \
        EXPECT_EQ(Key, item->data_);            \
      EXPECT_NE(true, trie.empty());            \
    }                                           \
    else                                        \
      EXPECT_EQ(nullptr, item);                 \
  } while (0)

# define INSERT(Key, Success)                                           \
  do {                                                                  \
    auto size = trie.size();                                            \
    EXPECT_EQ(Success, trie.insert(Value::Ptr(new Value(Key))));        \
    FIND(Key, Success);                                                 \
    EXPECT_EQ(size + Success, trie.size());                             \
  } while (0)

# define ERASE(Key, Success)                    \
  do {                                          \
    /* check if the container has the key */    \
    bool has = trie.find(Key);                  \
    EXPECT_EQ(Success, has);                    \
    auto size = trie.size();                    \
    EXPECT_EQ(Success, trie.erase(Key));        \
    /* check that we can't find the key */      \
    FIND(Key, false);                           \
    EXPECT_EQ(size - has, trie.size());         \
  } while (0)

namespace mimosa
{
  namespace
  {
    struct Value : public RefCountable<Value>
    {
      Value(const char *str) : data_(str) {}

      const char *data_;
    };

    StringRef getKey(Value::Ptr value)
    {
      return StringRef(value->data_);
    }

    TEST(Trie, Empty)
    {
      Trie<Value::Ptr, getKey> trie;

      EXPECT_EQ(true, trie.empty());
      EXPECT_EQ(0, trie.size());
      FIND("some key", false);
    }

    TEST(Trie, OneValue)
    {
      Trie<Value::Ptr, getKey> trie;

      INSERT("bonjour", true);
      ERASE("bonjour", true);
    }

    TEST(Trie, TwoValues)
    {
      Trie<Value::Ptr, getKey> trie;

      INSERT("a", true);
      INSERT("b", true);
      ERASE("b", true);
      ERASE("a", true);
    }

    TEST(Trie, ManyUnaryValues)
    {
      Trie<Value::Ptr, getKey> trie;

      INSERT("a", true);
      INSERT("b", true);
      ERASE("b", true);
      ERASE("a", true);

      INSERT("a", true);
      INSERT("b", true);
      INSERT("c", true);
      INSERT("d", true);
      INSERT("e", true);
      INSERT("f", true);
      ERASE("a", true);
      ERASE("b", true);
      ERASE("c", true);
      ERASE("d", true);
      ERASE("e", true);
      ERASE("f", true);
    }

    TEST(Trie, Depth)
    {
      Trie<Value::Ptr, getKey> trie;

      INSERT("", true);
      INSERT("a", true);
      INSERT("aa", true);
      INSERT("aaa", true);
      FIND("", true);
      FIND("a", true);
      FIND("aa", true);
      FIND("aaa", true);

      ERASE("aa", true);
      FIND("", true);
      FIND("a", true);
      FIND("aa", false);
      FIND("aaa", true);
    }
  }
}
