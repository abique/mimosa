#ifndef MIMOSA_SYNC_RWLOCK_HH
# define MIMOSA_SYNC_RWLOCK_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"
# include "locker.hh"
# include "rwlocker.hh"
# include "unique-locker.hh"

namespace mimosa
{
  namespace sync
  {
    class RWLock : private NonCopyable
    {
    public:
      typedef sync::Locker<RWLock>       Locker;
      typedef sync::RWLocker<RWLock>     ReadLocker;
      typedef sync::UniqueLocker<RWLock> UniqueLocker;

      inline RWLock()
        : lock_(::melon_rwlock_new())
      {
        if (!lock_)
          throw std::bad_alloc();
      }

      inline ~RWLock() { ::melon_rwlock_destroy(lock_); }
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
