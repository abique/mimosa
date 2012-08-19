#ifndef MIMOSA_INTRUSIVE_HASH_TABLE_HH
# define MIMOSA_INTRUSIVE_HASH_TABLE_HH

# include <cstdint>

# include "non-copyable.hh"

namespace mimosa
{
  template <typename Ptr>
  class IntrusiveHashTableHook
  {
  public:
    inline IntrusiveHashTableHook() : next_(nullptr), hash_(0) {}

    Ptr      next_;
    uint32_t hash_;
  };

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  class IntrusiveHashTable : public NonCopyable
  {
  public:

    inline IntrusiveHashTable(size_t capacity = 8);
    inline ~IntrusiveHashTable();
    inline IntrusiveHashTable(IntrusiveHashTable<T, Ptr, Member> && ht);
    inline IntrusiveHashTable & operator=(IntrusiveHashTable<T, Ptr, Member> && ht);

    inline bool empty() const { return size_ == 0; }
    inline size_t size() const { return size_; }
    inline size_t capacity() const { return capacity_; }

    inline void insert(Ptr entry);
    inline void rehash(size_t capacity);
    inline void remove(Ptr entry);
    inline void clear();

    template <typename Eq>
    inline Ptr find(uint32_t hash, const Eq & eq) const;

  private:
    Ptr    *buckets_;
    size_t  size_;
    size_t  capacity_;
  };
}

# include "intrusive-hash-table.hxx"

#endif /* !MIMOSA_INTRUSIVE_HASH_TABLE_HH */
