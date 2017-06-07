#pragma once

# include <cassert>
# include <cstddef>

# include "non-copyable.hh"

namespace mimosa
{
  template <typename Ptr>
  class IntrusiveSlistHook
  {
  public:
    inline IntrusiveSlistHook() : next_(nullptr) {}
    inline IntrusiveSlistHook(const IntrusiveSlistHook<Ptr> &) : next_(nullptr) {}
    inline IntrusiveSlistHook<Ptr> & operator=(const IntrusiveSlistHook<Ptr> &) { return *this; }

    Ptr next_;
  };

  template <typename T, typename Ptr, IntrusiveSlistHook<Ptr> T::*Member>
  class IntrusiveSlist;

  template <typename T, typename Ptr, IntrusiveSlistHook<Ptr> T::*Member>
  class IntrusiveSlistIterator
  {
  public:
    inline IntrusiveSlistIterator(const IntrusiveSlist<T, Ptr, Member> & slist, Ptr item)
      : slist_(slist), item_(item)
    {
    }

    inline T & operator*() const { return *item_; }
    inline Ptr operator->() const { return item_; }
    inline IntrusiveSlistIterator<T, Ptr, Member> & operator++()
    {
      assert(item_);
      if (item_ == slist_.tail_)
        item_ = nullptr;
      else
        item_ = (item_->*Member).next_;
      return *this;
    }

    inline bool operator==(const IntrusiveSlistIterator<T, Ptr, Member> & other) const
    {
      return &slist_ == &other.slist_ && item_ == other.item_;
    }

    inline bool operator!=(const IntrusiveSlistIterator<T, Ptr, Member> & other) const
    {
      return !(*this == other);
    }

  private:
    const IntrusiveSlist<T, Ptr, Member> & slist_;
    Ptr                                    item_;
  };

  template <typename T, typename Ptr, IntrusiveSlistHook<Ptr> T::*Member>
  class IntrusiveSlist : private NonCopyable
  {
  public:
    typedef IntrusiveSlistIterator<T, Ptr, Member> iterator;
    friend class IntrusiveSlistIterator<T, Ptr, Member>;

    inline IntrusiveSlist() : tail_(nullptr), size_(0) {}
    inline ~IntrusiveSlist()
    {
      while (!empty())
        pop();
    }

    inline bool empty() const
    {
      return !tail_;
    }

    inline size_t size() const
    {
      return size_;
    }

    inline void push(Ptr item)
    {
      ++size_;
      if (!tail_)
        (item->*Member).next_ = item;
      else
      {
        (item->*Member).next_  = (tail_->*Member).next_;
        (tail_->*Member).next_ = item;
      }
      tail_ = item;
    }

    inline Ptr front() const
    {
      assert(tail_);
      return (tail_->*Member).next_;
    }

    inline Ptr back() const
    {
      assert(tail_);
      return tail_;
    }

    inline void pop()
    {
      if (!tail_)
        return;
      --size_;
      if (tail_ == (tail_->*Member).next_)
      {
        (tail_->*Member).next_ = nullptr;
        tail_ = nullptr;
      }
      else
      {
        Ptr tmp                = (tail_->*Member).next_;
        (tail_->*Member).next_ = (tmp->*Member).next_;
        (tmp->*Member).next_   = nullptr;
      }
    }

    inline void clear()
    {
      // poping everything to unlink (and potentially release while unref)
      while (!empty())
        pop();
    }

    iterator begin() const { return empty() ? end() : iterator(*this, front()); }
    iterator end() const { return iterator(*this, nullptr); }

  private:
    Ptr    tail_;
    size_t size_;
  };
}

