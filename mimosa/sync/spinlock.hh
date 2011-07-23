#ifndef MIMOSA_SYNC_SPINLOCK_HH
# define MIMOSA_SYNC_SPINLOCK_HH

# include <melon/melon.h>

# include "../non-copyable.hh"
# include "locker.hh"
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
      inline SpinLock() { ::melon_spin_init(&spinlock_); }
      inline ~SpinLock() { ::melon_spin_destroy(spinlock_); }
      inline void lock() { ::melon_spin_lock(&spinlock_); }
      inline void unlock() { ::melon_spin_unlock(&spinlock_); }
    private:
      ::melon_mutex * spinlock_;
    };
  }
}

#endif /* !MIMOSA_SYNC_SPINLOCK_HH */
