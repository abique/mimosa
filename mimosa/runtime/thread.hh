#ifndef MIMOSA_RUNTIME_FIBER_HH
# define MIMOSA_RUNTIME_FIBER_HH

# include <functional>
# include <stdexcept>

# include <pthread.h>

# include "../non-copyable.hh"
# include "time.hh"

namespace mimosa
{
  namespace runtime
  {
    class Thread : private NonCopyable
    {
    public:

      template <typename T>
      static inline void start(void* (*fct)(T *), T * ctx)
      {
        pthread_t thread;
        if (::pthread_create(&thread, nullptr, reinterpret_cast<void*(*)(void*)>(fct),
                                      static_cast<void*>(ctx)))
          throw std::runtime_error("failed to start new fiber");
        ::pthread_detach(thread);
      }

      static void start(std::function<void ()> && fct);

      template <typename T>
      inline Thread(void* (*fct)(T *), T * ctx)
        : thread_(),
          is_detached_(false)
      {
        if (::pthread_create(&thread_, nullptr,
                                 reinterpret_cast<void*(*)(void*)>(fct),
                                 static_cast<void*>(ctx)))
          throw std::runtime_error("failed to start new fiber");
      }

      Thread(std::function<void ()> && fct);
      ~Thread();

      void join();
      bool tryJoin();
      bool timedJoin(Time timeout);
      void detach();

      inline pthread_t threadId() const { return thread_; }

    private:
      pthread_t thread_;
      bool      is_detached_;
    };
  }
}

#endif /* !MIMOSA_RUNTIME_FIBER_HH */
