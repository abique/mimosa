#ifndef MIMOSA_SYNC_MUTEX_HH
# define MIMOSA_SYNC_MUTEX_HH

# include <pthread.h>

# include <stdexcept>

# include "../runtime/time.hh"
# include "../non-copyable.hh"
# include "locker.hh"
# include "unique-locker.hh"

namespace mimosa
{
  namespace sync
  {
    class Mutex : private NonCopyable
    {
    public:
      typedef sync::Locker<Mutex> Locker;
      typedef sync::UniqueLocker<Mutex> UniqueLocker;

      inline Mutex() : mutex_()
      {
        ::pthread_mutex_init(&mutex_, nullptr);
      }

      inline ~Mutex() { ::pthread_mutex_destroy(&mutex_); }
      inline void lock() { ::pthread_mutex_lock(&mutex_); }
      inline void unlock() { ::pthread_mutex_unlock(&mutex_); }
      inline bool tryLock() { return !::pthread_mutex_trylock(&mutex_); }
      inline bool timedLock(runtime::Time time)
      {
        ::timespec tp;
        runtime::toTimeSpec(time, &tp);
        return !::pthread_mutex_timedlock(&mutex_, &tp);
      }

    private:
      friend class Condition;
      ::pthread_mutex_t mutex_;
    };
  }
}

#endif /* !MIMOSA_SYNC_MUTEX_HH */
