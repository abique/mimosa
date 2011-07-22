#ifndef MIMOSA_SPINLOCK_HH
# define MIMOSA_SPINLOCK_HH

# include <melon/melon.h>

namespace mimosa
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

#endif /* !MIMOSA_SPINLOCK_HH */
