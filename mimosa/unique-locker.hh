#pragma once

#include <cassert>

#include "non-copyable.hh"
#include "non-movable.hh"
#include "time.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  template <typename T>
  class UniqueLocker : private NonCopyable, private NonMovable
  {
  public:
    inline UniqueLocker(T& mutex, bool acquire = true)
      : mutex_(mutex),
        is_locked_(acquire)
    {
      if (acquire)
        mutex_.lock();
    }

    inline ~UniqueLocker()
    {
       if (is_locked_)
          mutex_.unlock();
    }

    inline void lock()
    {
      if (!is_locked_)
        mutex_.lock();
    }

    inline bool tryLock()
    {
      if (!is_locked_)
        is_locked_ = mutex_.trylock();
      return is_locked_;
    }

    inline bool timedLock(Time timeout)
    {
      if (!is_locked_)
        is_locked_ = mutex_.timedlock(timeout);
      return is_locked_;
    }

    inline void unlock()
    {
      assert(is_locked_);
      mutex_.unlock();
    }

  private:
    bool is_locked_ = false;
    T&   mutex_;
  };
}
