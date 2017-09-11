#pragma once

#include <new>
#include <cassert>
#include <cstdlib>

namespace mimosa
{
  //////////////////////////////////
  // CONSTRUCTORS AND DESTRUCTORS //
  //////////////////////////////////

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  IntrusiveHashTable<T, Ptr, Member>::IntrusiveHashTable(size_t capacity)
    : buckets_(nullptr),
      size_(0),
      capacity_(0)
  {
    rehash(capacity);
  }

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  IntrusiveHashTable<T, Ptr, Member>::IntrusiveHashTable(IntrusiveHashTable<T, Ptr, Member> && ht)
    : buckets_(ht.buckets_),
      size_(ht.size_),
      capacity_(ht.capacity_)
  {
    ht.buckets_  = nullptr;
    ht.size_     = 0;
    ht.capacity_ = 0;
  }

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  IntrusiveHashTable<T, Ptr, Member> &
  IntrusiveHashTable<T, Ptr, Member>::operator=(IntrusiveHashTable<T, Ptr, Member> && ht)
  {
    ::free(buckets_);
    buckets_     = ht.buckets_;
    size_        = ht.size_;
    capacity_    = ht.capacity_;
    ht.buckets_  = nullptr;
    ht.size_     = 0;
    ht.capacity_ = 0;
  }

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  IntrusiveHashTable<T, Ptr, Member>::~IntrusiveHashTable()
  {
    clear();
    free(buckets_);
  }

  ///////////////
  // MODIFIERS //
  ///////////////

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  void
  IntrusiveHashTable<T, Ptr, Member>::insert(Ptr entry, bool auto_rehash)
  {
    if (capacity_ == 0)
      rehash(8);

    auto & hook   = entry->*Member;
    auto & bucket = buckets_[hook.hash_ % capacity_];

    // ensure that the hook is not already used
    assert(!hook.next_);
    hook.next_ = bucket;
    bucket     = entry;
    ++size_;

    if (auto_rehash)
      rehash(0);
  }

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  void
  IntrusiveHashTable<T, Ptr, Member>::rehash(size_t capacity)
  {
    if (capacity == 0) {
      if (capacity_ < 8)
        capacity = 8;
      else if (size_ >= capacity_ || size_ * 2 < capacity_)
        capacity = (size_ * 3) / 2;
      else
        return;
    } else if (capacity_ == capacity)
      return;

    Ptr * new_buckets = reinterpret_cast<Ptr *> (calloc(sizeof (*new_buckets), capacity));
    if (!new_buckets)
      throw std::bad_alloc();

    if (size_ > 0) {
      for (size_t i = 0; i < capacity_; ++i) {
        while (buckets_[i]) {
          auto   ptr        = buckets_[i];
          auto & hook       = ptr->*Member;
          buckets_[i]       = hook.next_;
          auto & new_bucket = new_buckets[hook.hash_ % capacity];
          hook.next_        = new_bucket;
          new_bucket        = ptr;
        }
      }
    }

    free(buckets_);
    buckets_  = new_buckets;
    capacity_ = capacity;
  }

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  void
  IntrusiveHashTable<T, Ptr, Member>::remove(Ptr entry, bool auto_rehash)
  {
    if (!buckets_)
      return;

    auto & hook = entry->*Member;
    auto   ptr  = buckets_[hook.hash_ % capacity_];
    Ptr *  prev = buckets_ + (hook.hash_ % capacity_);

    while (ptr) {
      if (ptr == entry) {
        *prev      = hook.next_;
        hook.next_ = nullptr;
        --size_;
        break;
      }
      prev = &(ptr->*Member).next_;
      ptr  = (ptr->*Member).next_;
    }

    if (auto_rehash)
      rehash(0);
  }

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  void
  IntrusiveHashTable<T, Ptr, Member>::clear()
  {
    for (size_t i = 0; i < capacity_; ++i) {
      while (buckets_[i]) {
        auto   ptr        = buckets_[i];
        auto & hook       = ptr->*Member;
        buckets_[i]       = hook.next_;
        hook.next_        = nullptr;
      }
    }

    free(buckets_);
    buckets_  = nullptr;
    capacity_ = 0;
    size_     = 0;
  }

  //////////////////////
  // FIND & ITERATORS //
  //////////////////////

  template <typename T, typename Ptr, IntrusiveHashTableHook<Ptr> T::*Member>
  template <typename Eq>
  Ptr
  IntrusiveHashTable<T, Ptr, Member>::find(uint32_t hash, const Eq & eq) const
  {
    if (size_ == 0)
      return nullptr;

    for (Ptr it = buckets_[hash % capacity_]; it; it = (it->*Member).next_)
      if ((it->*Member).hash_ == hash && eq(it))
        return it;
    return nullptr;
  }
}
