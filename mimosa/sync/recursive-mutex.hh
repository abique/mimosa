#ifndef MIMOSA_SYNC_RECURSIVE_MUTEX_HH
# define MIMOSA_SYNC_RECURSIVE_MUTEX_HH

# include <melon/melon.h>
# include "../non-copyable.hh"
# include "locker.hh"
# include "unique-locker.hh"

namespace mimosa
{
  namespace sync
  {
    class RecursiveMutex : private NonCopyable
    {
    public:
      typedef Locker<RecursiveMutex> Locker;
      typedef UniqueLocker<RecursiveMutex> UniqueLocker;

      inline RecursiveMutex() throw std::bad_alloc : mutex_(nullptr)
      {
        ::melon_mutexattr * attr = nullptr;
        if (::melon_mutexattr_init(&attr))
          throw std::bad_alloc;

        ::melon_mutexattr_settype(attr, MELON_MUTEX_RECCURSIVE);
        int ret = ::melon_mutex_init(&mutex_, attr);
        ::melon_mutexattr_destroy(attr);

        if (ret)
          throw std::bad_alloc;
      }

      inline ~RecursiveMutex() { ::melon_mutex_destroy(mutex_); }
      inline void lock() { ::melon_mutex_lock(mutex_); }
      inline void unlock() { ::melon_mutex_unlock(mutex_); }
      inline bool tryLock() { return !::melon_mutex_trylock(mutex_); }
      inline bool timedLock(::melon_time_t time) { return !::melon_mutex_timedlock(mutex_, time); }
    private:
      friend class Condition;
      ::melon_mutex * mutex_;
    };
  }
}

#endif /* !MIMOSA_SYNC_RECURSIVE_MUTEX_HH */
