#ifndef MIMOSA_RUNTIME_FIBER_HH
# define MIMOSA_RUNTIME_FIBER_HH

# include <melon/melon.h>

# include "non-copyable.hh"

namespace mimosa
{
  namespace runtime
  {
    class Fiber : private NonCopyable
    {
    public:

      template <typename T>
      static inline bool startFiber(void (*fct)(T *), T * ctx)
      {
        return !::melon_fiber_startlight((void(*)(void*))fct, (void*)ctx);
      }

    };
  }
}

#endif /* !MIMOSA_RUNTIME_FIBER_HH */
