#include <gtest/gtest.h>

#include "../intrusive-hash-table.hh"

namespace mimosa
{
  namespace
  {
    struct Item
    {
      IntrusiveHashTableHook<Item *> hook_;
      uint32_t                       value_;

      inline Item(uint32_t value)
      {
        set(value);
      }

      inline void set(uint32_t value)
      {
        value_ = value;
        hook_.hash_ = value + 5;
      }
    };

    typedef IntrusiveHashTable<Item, Item *, &Item::hook_> MyHashTable;

    TEST(IntrusiveHashTable, Simple)
    {
      MyHashTable ht(3);

      EXPECT_EQ(ht.size(), 0);
      EXPECT_EQ(ht.empty(), true);
      EXPECT_EQ(ht.capacity(), 3);

      auto item = new Item(1);
      ht.insert(item);

      EXPECT_EQ(ht.size(), 1);
      EXPECT_EQ(ht.empty(), false);
      EXPECT_EQ(ht.capacity(), 3);

      auto item2 = ht.find(item->hook_.hash_, [] (Item *) { return true; });
      EXPECT_EQ(item2, item);

      ht.remove(item);

      EXPECT_EQ(ht.size(), 0);
      EXPECT_EQ(ht.empty(), true);
      EXPECT_EQ(ht.capacity(), 3);

      delete item;
    }

    TEST(IntrusiveHashTable, Colision)
    {
      MyHashTable ht(1);

      EXPECT_EQ(ht.size(), 0);
      EXPECT_EQ(ht.empty(), true);
      EXPECT_EQ(ht.capacity(), 1);

      Item * items[5];
      for (int i = 0; i < 5; ++i) {
        items[i] = new Item(i);
        ht.insert(items[i]);
        EXPECT_EQ(ht.size(), i + 1);
        EXPECT_EQ(ht.empty(), false);
        EXPECT_EQ(ht.capacity(), 1);
      }

      for (int i = 0; i < 5; ++i) {
        auto item2 = ht.find(items[i]->hook_.hash_, [] (Item *) { return true; });
        EXPECT_EQ(item2, items[i]);
      }

      for (int i = 0; i < 5; ++i) {
        ht.remove(items[i]);
        EXPECT_EQ(ht.size(), 5 - i - 1);
      }

      EXPECT_EQ(ht.size(), 0);
      EXPECT_EQ(ht.empty(), true);
      EXPECT_EQ(ht.capacity(), 1);

      for (int i = 0; i < 5; ++i)
        delete items[i];
    }
  }
}
