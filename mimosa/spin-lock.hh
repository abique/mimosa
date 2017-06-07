#pragma once

#include <pthread.h>

#include "non-copyable.hh"
#include "locker.hh"
#include "unique-locker.hh"

namespace mimosa
{
  /**
   * SpinLock is a user level lock which will actively loop until it can
   * acquire the lock.
   *
   * @warning don't use it in situation where the lock ownership is longer
   * than 50us (mode switch).
   *
   * @ingroup Sync
   */
  class SpinLock : private NonCopyable
  {
  public:
    typedef mimosa::Locker<SpinLock> Locker;
    typedef mimosa::UniqueLocker<SpinLock> UniqueLocker;

    inline SpinLock();
    inline ~SpinLock();
    inline void lock();
    inline bool tryLock();
    inline void unlock();

  private:
    int lock_;
  };
}

#include "spinlock.hxx"
