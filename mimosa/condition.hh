﻿#pragma once

#include <pthread.h>

#include <stdexcept>

#include "non-copyable.hh"
#include "non-movable.hh"
#include "time.hh"
#include "mutex.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  class Condition : private NonCopyable, private NonMovable
  {
  public:
    inline Condition()
      : cond_()
    {
      ::pthread_condattr_t attr;

      if (::pthread_condattr_init(&attr))
        throw std::bad_alloc();

#ifndef __MACH__
      if (::pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) ||
          ::pthread_cond_init(&cond_, &attr))
      {
        ::pthread_condattr_destroy(&attr);
        throw std::bad_alloc();
      }
#endif

      ::pthread_condattr_destroy(&attr);
    }

    inline ~Condition() { ::pthread_cond_destroy(&cond_); }

    template <typename Mutex>
    inline void wait(Mutex & mutex)
    {
      ::pthread_cond_wait(&cond_, &mutex.nativeHandle());
    }

    /**
     * @return true if succeed, false if timeout
     */
    inline bool timedWait(Mutex & mutex, Time time)
    {
      ::timespec tp = toTimeSpec(time);
      return !::pthread_cond_timedwait(&cond_, &mutex.nativeHandle(), &tp);
    }

    inline void wakeOne() { ::pthread_cond_signal(&cond_); }
    inline void wakeAll() { ::pthread_cond_broadcast(&cond_); }

  private:
    ::pthread_cond_t cond_;
  };
}

