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
  class Mutex : private NonCopyable, private NonMovable
  {
  public:
    typedef mimosa::Locker<Mutex> Locker;
    typedef mimosa::UniqueLocker<Mutex> UniqueLocker;

    inline Mutex()
    {
      if (::pthread_mutex_init(&mutex_, nullptr))
        throw std::bad_alloc();
    }

    inline ~Mutex()
    {
       ::pthread_mutex_destroy(&mutex_);
    }

    inline void lock()
    {
       if (::pthread_mutex_lock(&mutex_))
          throw std::runtime_error("pthread_mutex_lock failed");
    }

    inline void unlock()
    {
       if (::pthread_mutex_unlock(&mutex_))
          throw std::runtime_error("pthread_mutex_unlock failed");
    }

    inline bool tryLock()
    {
       return !::pthread_mutex_trylock(&mutex_);
    }

    inline ::pthread_mutex_t& nativeHandle()
    {
      return mutex_;
    }

    // find a way to use monotonic clock here
    // inline bool timedLock(Time time)
    // {
    //   ::timespec tp;
    //   toTimeSpec(time, &tp);
    //   return !::pthread_mutex_timedlock(&mutex_, &tp);
    // }

  private:
    ::pthread_mutex_t mutex_;
  };
}

