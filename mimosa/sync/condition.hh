#ifndef MIMOSA_SYNC_CONDITION_HH
# define MIMOSA_SYNC_CONDITION_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"
# include "../runtime/time.hh"

namespace mimosa
{
  namespace sync
  {
    class Condition : private NonCopyable
    {
    public:
      inline Condition()
        : cond_(::melon_cond_new())
      {
        if (!cond_)
          throw std::bad_alloc();
      }

      inline ~Condition() { ::melon_cond_destroy(cond_); }

      template <typename Mutex>
      inline void wait(Mutex & mutex, runtime::Time time = 0)
      { ::melon_cond_timedwait(cond_, mutex.mutex_, time); }

      inline void wakeOne() { ::melon_cond_signal(cond_); }
      inline void wakeAll() { ::melon_cond_broadcast(cond_); }
    private:
      ::melon_cond * cond_;
    };
  }
}

#endif /* !MIMOSA_SYNC_CONDITION_HH */
