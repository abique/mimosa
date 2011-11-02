#ifndef MIMOSA_SYNC_SPINLOCK_HH
# define MIMOSA_SYNC_SPINLOCK_HH

# include <pthread.h>

# include "../non-copyable.hh"
# include "locker.hh"

namespace mimosa
{
  namespace sync
  {
    class SpinLock : private NonCopyable
    {
    public:
      typedef Locker<SpinLock> Locker;
      typedef UniqueLocker<SpinLock> UniqueLocker;
      inline SpinLock() { ::pthread_spin_init(&spinlock_, PTHREAD_PROCESS_PRIVATE); }
      inline ~SpinLock() { ::pthread_spin_destroy(spinlock_); }
      inline void lock() { ::pthread_spin_lock(&spinlock_); }
      inline void unlock() { ::pthread_spin_unlock(&spinlock_); }
    private:
      ::pthread_spinlock_t spinlock_;
    };
  }
}

#endif /* !MIMOSA_SYNC_SPINLOCK_HH */
