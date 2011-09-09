#ifndef MIMOSA_SYNC_SEMAPHORE_HH
# define MIMOSA_SYNC_SEMAPHORE_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace sync
  {
    class Semaphore : private NonCopyable
    {
    public:
      inline Semaphore(int nb) throw std::bad_alloc
        : sem_(nullptr)
      {
        if (::melon_sem_new(&sem_, nullptr, nb))
          throw std::bad_alloc;
      }

      inline ~Semaphore()
      {
        ::melon_sem_destroy(sem_);
      }

      inline void acquire(int nb) { ::melon_sem_acquire(sem_, nb); }
      inline bool tryAcquire(int nb) { return !::melon_sem_tryacquire(sem_, nb); }
      inline bool timedAcquire(int nb) { return !::melon_sem_timedacquire(sem_, nb); }
      inline void release(int nb) { ::melon_sem_release(sem_, nb); }
    };
  }
}

#endif /* !MIMOSA_SYNC_SEMAPHORE_HH */
