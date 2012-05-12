#ifndef MIMOSA_YIELD_HH
# define MIMOSA_YIELD_HH

# include <sched.h>

namespace mimosa
{
  inline void yield() { ::sched_yield(); }
}

#endif /* !MIMOSA_YIELD_HH */
