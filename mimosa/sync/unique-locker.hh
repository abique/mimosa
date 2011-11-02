#ifndef MIMOSA_SYNC_UNIQUE_LOCKER_HH
# define MIMOSA_SYNC_UNIQUE_LOCKER_HH

# include <cassert>

# include "../non-copyable.hh"
# include "../runtime/time.hh"

namespace mimosa
{
  namespace sync
  {
    template <typename T>
    class UniqueLocker : private NonCopyable
    {
    public:
      inline UniqueLocker(T & mutex, bool acquire = true)
        : mutex_(mutex),
          is_locked_(acquire)
      {
        if (acquire)
          mutex_.lock();
      }

      inline ~UniqueLocker() { if (is_locked_) mutex_.unlock(); }
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

      inline bool timedLock(runtime::Time timeout)
      {
        if (!is_locked_)
          is_locked_ = mutex_.timedlock(time);
        return is_locked_;
      }

      inline void unlock()
      {
        assert(is_locked_);
        mutex_.unlock();
      }

    private:
      bool is_locked_;
      T &  mutex_;
    };
  }
}

#endif /* !MIMOSA_SYNC_UNIQUE_LOCKER_HH */
