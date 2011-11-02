#ifndef MIMOSA_RUNTIME_YIELD_HH
# define MIMOSA_RUNTIME_YIELD_HH

# include <sched.h>

namespace mimosa
{
  namespace runtime
  {
    inline void yield() { ::sched_yield(); }
  }
}

#endif /* !MIMOSA_RUNTIME_YIELD_HH */
