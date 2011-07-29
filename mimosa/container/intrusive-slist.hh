#ifndef MIMOSA_CONTAINER_INTRUSIVE_SLIST_HH
# define MIMOSA_CONTAINER_INTRUSIVE_SLIST_HH

# include <cassert>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace container
  {
    template <typename T>
    class IntrusiveSlistHook
    {
    public:
      inline IntrusiveSlistHook() : next_(nullptr) {}
      inline IntrusiveSlistHook(const IntrusiveSlistHook<T> &) : next_(nullptr) {}
      inline IntrusiveSlistHook<T> & operator=(const IntrusiveSlistHook<T> &) { return *this; }

      T * next_;
    };

    template <typename T, IntrusiveSlistHook<T> T::*Member>
    class IntrusiveSlist;

    template <typename T, IntrusiveSlistHook<T> T::*Member>
    class IntrusiveSlistIterator
    {
    public:
      inline IntrusiveSlistIterator(const IntrusiveSlist<T, Member> & slist, T * item)
        : slist_(slist), item_(item)
      {
      }

      inline T & operator*() const { return *item_; }
      inline T * operator->() const { return item_; }
      inline IntrusiveSlistIterator<T, Member> & operator++()
      {
        assert(item_);
        if ((item_->*Member).next_ == item_ ||
            item_ == slist_.tail_)
          item_ = nullptr;
        else
          item_ = (item_->*Member).next_;
        return *this;
      }

      inline bool operator==(const IntrusiveSlistIterator<T, Member> & other) const
      {
        return &slist_ == &other.slist_ && item_ == other.item_;
      }

      inline bool operator!=(const IntrusiveSlistIterator<T, Member> & other) const
      {
        return !(*this == other);
      }

    private:
      const IntrusiveSlist<T, Member> & slist_;
      T *                               item_;
    };

    template <typename T, IntrusiveSlistHook<T> T::*Member>
    class IntrusiveSlist : private NonCopyable
    {
    public:
      typedef IntrusiveSlistIterator<T, Member> iterator;
      friend class IntrusiveSlistIterator<T, Member>;

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

      inline void push(T & item)
      {
        ++size_;
        if (!tail_)
          (item.*Member).next_ = &item;
        else
        {
          (item.*Member).next_   = (tail_->*Member).next_;
          (tail_->*Member).next_ = &item;
        }
        tail_ = &item;
      }

      inline T & front() const
      {
        assert(tail_);
        return *(tail_->*Member).next_;
      }

      inline T & back() const
      {
        assert(tail_);
        return *tail_;
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
          T * tmp                = (tail_->*Member).next_;
          (tail_->*Member).next_ = (tmp->*Member).next_;
          (tmp->*Member).next_   = nullptr;
        }
      }

      iterator begin() const { return iterator(*this, (tail_->*Member).next_); }
      iterator end() const { return iterator(*this, nullptr); }

    private:
      T *    tail_;
      size_t size_;
    };
  }
}

#endif /* !MIMOSA_CONTAINER_INTRUSIVE_SLIST_HH */
