#ifndef MIMOSA_RUNTIME_YIELD_HH
# define MIMOSA_RUNTIME_YIELD_HH

namespace mimosa
{
  namespace runtime
  {
    inline void yield() { ::melon_yield(); }
  }
}

#endif /* !MIMOSA_RUNTIME_YIELD_HH */
