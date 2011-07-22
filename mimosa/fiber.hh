#ifndef MIMOSA_FIBER_HH
# define MIMOSA_FIBER_HH

# include <melon/melon.h>

namespace mimosa
{
  template <typename T>
  inline bool startFiber(void (*fct)(T *), T * ctx)
  {
    return !::melon_fiber_startlight((void(*)(void*))fct, (void*)ctx);
  }
}

#endif /* !MIMOSA_FIBER_HH */
