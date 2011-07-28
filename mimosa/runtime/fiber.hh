#ifndef MIMOSA_RUNTIME_FIBER_HH
# define MIMOSA_RUNTIME_FIBER_HH

# define restrict

# include <functional>

# include <melon/melon.h>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace runtime
  {
    class Fiber : private NonCopyable
    {
    public:

      template <typename T>
      static inline bool start(void* (*fct)(T *), T * ctx)
      {
        return !::melon_fiber_startlight(reinterpret_cast<void*(*)(void*)>(fct),
                                         static_cast<void*>(ctx));
      }

      static bool start(std::function<void ()> && fct);
    };
  }
}

#endif /* !MIMOSA_RUNTIME_FIBER_HH */
