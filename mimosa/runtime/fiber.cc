#include "fiber.hh"

namespace mimosa
{
  namespace runtime
  {
    static void* startWrapper(std::function<void ()> * fct)
    {
      (*fct)();
      delete fct;
      return 0;
    }

    void Fiber::start(std::function<void ()> && fct)
    {
      auto cb = new std::function<void ()>(fct);
      Fiber::start(startWrapper, cb);
    }

    Fiber::Fiber(std::function<void ()> && fct)
      : thread_(),
        is_detached_(false)
    {
      auto cb = new std::function<void ()>(fct);
      if (::pthread_create(&thread_, nullptr,
                               reinterpret_cast<void*(*)(void*)>(startWrapper),
                               static_cast<void*>(cb)))
        throw std::runtime_error("failed to start new fiber");
    }

    Fiber::~Fiber()
    {
      detach();
    }

    void Fiber::join()
    {
      if (!is_detached_)
        return;
      ::pthread_join(thread_, NULL);
      is_detached_ = true;
    }

    bool Fiber::tryJoin()
    {
      if (is_detached_)
        return false;

      if (::pthread_tryjoin_np(thread_, NULL))
        return false;
      is_detached_ = true;
      return true;
    }

    bool Fiber::timedJoin(Time timeout)
    {
      if (is_detached_)
        return false;

      ::timespec tp;
      toTimeSpec(timeout, &tp);
      if (::pthread_timedjoin_np(thread_, NULL, &tp))
        return false;
      is_detached_ = true;
      return true;
    }

    void Fiber::detach()
    {
      if (is_detached_)
        return;
      ::pthread_detach(thread_);
    }
  }
}
