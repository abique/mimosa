#ifndef MIMOSA_SYNC_RWLOCK_HH
# define MIMOSA_SYNC_RWLOCK_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable"
# include "locker.hh"
# include "rwlocker.hh"
# include "unique-locker.hh"

namespace mimosa
{
  namespace sync
  {
    class RWLock : public NonCopyable
    {
    public:
      typedef Locker<RwLock>       Locker;
      typedef RWLocker<RwLock>     ReadLocker;
      typedef UniqueLocker<RwLock> UniqueLocker;

      inline RWLock() throw std::bad_alloc
        : lock_(::melon_rwlock_new())
      {
        if (!lock_)
          throw std::bad_alloc;
      }

      inline ~RwLock() { ::melon_mutex_destroy(lock_); }
      inline void readLock() { ::melon_rwlock_rdlock(lock_); }
      inline void tryReadLock() { ::melon_rwlock_tryrdlock(lock_); }
      inline bool timedReadLock(::melon_time_t time) { return ::melon_rwlock_timedrdlock(lock_, time); }
      inline void lock() { ::melon_rwlock_wrlock(lock_); }
      inline void tryLock() { ::melon_rwlock_trywrlock(lock_); }
      inline bool timedLock(::melon_time_t time) { return ::melon_rwlock_timedwrlock(lock_, time); }
      inline void unlock() { ::melon_rwlock_unlock(lock_); }
    private:
      ::melon_rwlock * lock_;
    };
  }
}

#endif /* !MIMOSA_SYNC_RWLOCK_HH */
