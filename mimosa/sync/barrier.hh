#ifndef MIMOSA_SYNC_BARRIER_HH
# define MIMOSA_SYNC_BARRIER_HH

# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace sync
  {
    class Barrier : private NonCopyable
    {
    public:
      inline Barrier(uint16_t nb) throw std::bad_alloc
        : barrier_(nullptr)
      {
        if (::melon_barrier_init(&barrier_, nullptr, nb))
          throw std::bad_alloc;
      }

      inline ~Barrier() { ::melon_barrier_destroy(barrier_); }
      inline void add(uint16_t nb = 1) { ::melon_barrier_add(barrier_, nb); }
      inline void release(uint16_t nb = 1) { ::melon_barrier_release(barrier_, nb); }
      inline void wait() { ::melon_barrier_wait(barrier_); }

    private:
      melon_barrier * barrier_;
    };
  }
}

#endif /* !MIMOSA_SYNC_BARRIER_HH */
