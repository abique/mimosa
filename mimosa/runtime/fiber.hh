#ifndef MIMOSA_RUNTIME_FIBER_HH
# define MIMOSA_RUNTIME_FIBER_HH

# include <functional>
# include <stdexcept>

# include <melon/melon.h>

# include "../non-copyable.hh"
# include "time.hh"

namespace mimosa
{
  namespace runtime
  {
    class Fiber : private NonCopyable
    {
    public:

      template <typename T>
      static inline void start(void* (*fct)(T *), T * ctx)
      {
        if (::melon_fiber_createlight(nullptr, reinterpret_cast<void*(*)(void*)>(fct),
                                      static_cast<void*>(ctx)))
          throw std::runtime_error("failed to start new fiber");
      }

      static void start(std::function<void ()> && fct);

      template <typename T>
      inline Fiber(void* (*fct)(T *), T * ctx)
        : fiber_(nullptr)
      {
        if (::melon_fiber_create(&fiber_, nullptr,
                                 reinterpret_cast<void*(*)(void*)>(fct),
                                 static_cast<void*>(ctx)))
          throw std::runtime_error("failed to start new fiber");
      }

      Fiber(std::function<void ()> && fct);
      ~Fiber();

      void join();
      bool tryJoin();
      bool timedJoin(Time timeout);
      void detach();

    private:
      melon_fiber * fiber_;
    };
  }
}

#endif /* !MIMOSA_RUNTIME_FIBER_HH */
