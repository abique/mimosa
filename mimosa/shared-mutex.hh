#pragma once

#include <pthread.h>

#include <stdexcept>

#include "non-copyable.hh"
#include "non-movable.hh"
#include "time.hh"
#include "locker.hh"
#include "shared-locker.hh"
#include "unique-locker.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  class SharedMutex : private NonCopyable, private NonMovable
  {
  public:
    typedef mimosa::Locker<SharedMutex>       Locker;
    typedef mimosa::SharedLocker<SharedMutex> ReadLocker;
    typedef mimosa::UniqueLocker<SharedMutex> UniqueLocker;

    inline SharedMutex()
      : lock_()
    {
      if (::pthread_rwlock_init(&lock_, nullptr))
        throw std::bad_alloc();
    }

    inline ~SharedMutex()
    {
      ::pthread_rwlock_destroy(&lock_);
    }

    inline void sharedLock()
    {
      ::pthread_rwlock_rdlock(&lock_);
    }

    inline void trySharedLock()
    {
      ::pthread_rwlock_tryrdlock(&lock_);
    }

#ifndef __MACH__
    inline bool timedSharedLock(Time time)
    {
      ::timespec tp = toTimeSpec(time);
      return ::pthread_rwlock_timedrdlock(&lock_, &tp);
    }
#endif

    inline void lock()
    {
      ::pthread_rwlock_wrlock(&lock_);
    }

    inline void tryLock()
    {
      ::pthread_rwlock_trywrlock(&lock_);
    }

#ifndef __MACH__
    inline bool timedLock(Time time)
    {
      ::timespec tp = toTimeSpec(time);
      return ::pthread_rwlock_timedwrlock(&lock_, &tp);
    }
#endif

    inline void unlock()
    {
      ::pthread_rwlock_unlock(&lock_);
    }

  private:
    ::pthread_rwlock_t lock_;
  };
}

