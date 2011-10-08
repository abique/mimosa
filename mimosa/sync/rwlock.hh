#ifndef MIMOSA_SYNC_RWLOCK_HH
# define MIMOSA_SYNC_RWLOCK_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"
# include "../runtime/time.hh"
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
        : lock_()
      {
        if (::pthread_rwlock_init(&lock_, nullptr))
          throw std::bad_alloc();
      }

      inline ~RWLock()
      {
        ::pthread_rwlock_destroy(&lock_);
      }

      inline void readLock()
      {
        ::pthread_rwlock_rdlock(&lock_);
      }

      inline void tryReadLock()
      {
        ::pthread_rwlock_tryrdlock(&lock_);
      }

      inline bool timedReadLock(runtime::Time time)
      {
        ::timespec tp;
        runtime::toTimeSpec(time, &tp);
        return ::pthread_rwlock_timedrdlock(&lock_, &tp);
      }

      inline void lock()
      {
        ::pthread_rwlock_wrlock(&lock_);
      }

      inline void tryLock()
      {
        ::pthread_rwlock_trywrlock(&lock_);
      }

      inline bool timedLock(runtime::Time time)
      {
        ::timespec tp;
        runtime::toTimeSpec(time, &tp);
        return ::pthread_rwlock_timedwrlock(&lock_, &tp);
      }

      inline void unlock()
      {
        ::pthread_rwlock_unlock(&lock_);
      }

    private:
      ::pthread_rwlock_t lock_;
    };
  }
}

#endif /* !MIMOSA_SYNC_RWLOCK_HH */
