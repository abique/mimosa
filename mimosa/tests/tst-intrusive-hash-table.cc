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
    };

    typedef IntrusiveHashTable<Item, Item *, &Item::hook_> MyHashTable;

    TEST(IntrusiveHashTable, Compil)
    {
      MyHashTable ht;
    }
  }
}
