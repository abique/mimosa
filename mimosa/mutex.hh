#ifndef MIMOSA_MUTEX_HH
# define MIMOSA_MUTEX_HH

# include <pthread.h>

# include <stdexcept>

# include "time.hh"
# include "non-copyable.hh"
# include "locker.hh"
# include "unique-locker.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  class Mutex : private NonCopyable
  {
  public:
    typedef mimosa::Locker<Mutex> Locker;
    typedef mimosa::UniqueLocker<Mutex> UniqueLocker;

    inline Mutex() : mutex_()
    {
      if (::pthread_mutex_init(&mutex_, nullptr))
        throw std::bad_alloc();
    }

    inline ~Mutex() { ::pthread_mutex_destroy(&mutex_); }
    inline void lock() { ::pthread_mutex_lock(&mutex_); }
    inline void unlock() { ::pthread_mutex_unlock(&mutex_); }
    inline bool tryLock() { return !::pthread_mutex_trylock(&mutex_); }

    // find a way to use monotonic clock here
    // inline bool timedLock(Time time)
    // {
    //   ::timespec tp;
    //   toTimeSpec(time, &tp);
    //   return !::pthread_mutex_timedlock(&mutex_, &tp);
    // }

  private:
    friend class Condition;
    ::pthread_mutex_t mutex_;
  };
}

#endif /* !MIMOSA_MUTEX_HH */
