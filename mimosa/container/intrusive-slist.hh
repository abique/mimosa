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
      inline IntrusiveSlistHook() : next_(0) {}
      inline IntrusiveSlistHook(const IntrusiveSlistHook<T> &) : next_(0) {}
      inline IntrusiveSlistHook<T> & operator=(const IntrusiveSlistHook<T> &) { return *this; }

      T * next_;
    };

    template <typename T, IntrusiveSlistHook<T> T::*Member>
    class IntrusiveSlist : private NonCopyable
    {
    public:
      inline IntrusiveSlist() : tail_(0) {}
      inline IntrusiveSlist()
      {
        while (!empty())
          pop();
      }

      inline bool empty() const
      {
        return !tail_;
      }

      inline void push(T & item)
      {
        if (!tail_)
          item.*Member.next_ = &item;
        else
        {
          item.*Member.next_ = tail_->*Member.next_;
          tail_->*Member.next_ = &item;
        }
        tail_ = &item;
      }

      inline T & front() const
      {
        assert(tail_);
        return tail_->*Member.next_;
      }

      inline void pop()
      {
        if (!tail_)
          return;
        if (tail_ == tail_->*Member.next_)
        {
          tail_->*Member.next_ = 0;
          tail_ = 0;
        }
        else
        {
          T * tmp            = tail_->*Member.next;
          tail_->*Member.next = tmp->*Member.next;
          tmp->*Member.next   = 0;
        }
      }

    private:
      T * tail_;
    };
  }
}

#endif /* !MIMOSA_CONTAINER_INTRUSIVE_SLIST_HH */
