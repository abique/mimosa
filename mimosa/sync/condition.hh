#ifndef MIMOSA_SYNC_CONDITION_HH
# define MIMOSA_SYNC_CONDITION_HH

# include <pthread.h>

# include <stdexcept>

# include "../non-copyable.hh"
# include "../runtime/time.hh"
# include "mutex.hh"

namespace mimosa
{
  namespace sync
  {
    class Condition : private NonCopyable
    {
    public:
      inline Condition()
        : cond_()
      {
        if (::pthread_cond_init(&cond_, nullptr))
          throw std::bad_alloc();
      }

      inline ~Condition() { ::pthread_cond_destroy(&cond_); }

      template <typename Mutex>
      inline void wait(Mutex & mutex)
      {
        ::pthread_cond_wait(&cond_, &mutex.mutex_);
      }

      inline void timedWait(Mutex & mutex, runtime::Time time)
      {
        ::timespec tp;
        runtime::toTimeSpec(time, &tp);
        ::pthread_cond_timedwait(&cond_, &mutex.mutex_, &tp);
      }

      inline void wakeOne() { ::pthread_cond_signal(&cond_); }
      inline void wakeAll() { ::pthread_cond_broadcast(&cond_); }

    private:
      ::pthread_cond_t cond_;
    };
  }
}

#endif /* !MIMOSA_SYNC_CONDITION_HH */
