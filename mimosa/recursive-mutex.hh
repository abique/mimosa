#pragma once

#include <pthread.h>

#include <stdexcept>
#include <new>

#include "time.hh"
#include "non-copyable.hh"
#include "non-movable.hh"
#include "locker.hh"
#include "unique-locker.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  class RecursiveMutex : private NonCopyable, public NonMovable
  {
  public:
    typedef Locker<RecursiveMutex> Locker;
    typedef UniqueLocker<RecursiveMutex> UniqueLocker;

    inline RecursiveMutex()
      : mutex_(nullptr)
    {
      ::pthread_mutexattr_t attr;

      ::pthread_mutexattr_init(&attr);
      ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

      int ret = ::pthread_mutex_init(&mutex_, &attr);
      ::pthread_mutexattr_destroy(&attr);

      if (ret)
        throw std::bad_alloc();
    }

    inline ~RecursiveMutex() { ::pthread_mutex_destroy(&mutex_); }
    inline void lock() { ::pthread_mutex_lock(&mutex_); }
    inline void unlock() { ::pthread_mutex_unlock(&mutex_); }
    inline bool tryLock() { return !::pthread_mutex_trylock(&mutex_); }

  private:
    friend class Condition;
    ::pthread_mutex_t mutex_;
  };
}

