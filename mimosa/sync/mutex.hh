#ifndef MIMOSA_SYNC_MUTEX_HH
# define MIMOSA_SYNC_MUTEX_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"
# include "locker.hh"

namespace mimosa
{
  namespace sync
  {
    class Mutex : private NonCopyable
    {
    public:
      typedef Locker<Mutex> Locker;
      typedef UniqueLocker<Mutex> UniqueLocker;

      inline Mutex() throw std::bad_alloc : mutex_(::melon_mutex_new(0))
      {
        if (!mutex_)
          throw std::bad_alloc;
      }

      inline ~Mutex() { ::melon_mutex_destroy(mutex_); }
      inline void lock() { ::melon_mutex_lock(mutex_); }
      inline void unlock() { ::melon_mutex_lock(mutex_); }
      inline bool tryLock() { return !::melon_mutex_trylock(mutex_); }
      inline bool timedLock(::melon_time_t time) { return !::melon_mutex_timedlock(mutex_, time); }
    private:
      friend class Condition;
      ::melon_mutex * mutex_;
    };
  }
}

#endif /* !MIMOSA_SYNC_MUTEX_HH */
