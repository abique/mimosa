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
      : fiber_(0)
    {
      auto cb = new std::function<void ()>(fct);
      fiber_ = ::melon_fiber_start(reinterpret_cast<void*(*)(void*)>(startWrapper),
                                   static_cast<void*>(cb));
      if (!fiber_)
        throw std::runtime_error("failed to start new fiber");
    }

    Fiber::~Fiber()
    {
      detach();
    }

    void Fiber::join()
    {
      if (!fiber_)
        return;
      ::melon_fiber_join(fiber_, NULL);
      fiber_ = 0;
    }

    bool Fiber::tryJoin()
    {
      if (!fiber_)
        return false;

      if (::melon_fiber_tryjoin(fiber_, NULL))
        return false;
      fiber_ = 0;
      return true;
    }

    bool Fiber::timedJoin(Time timeout)
    {
      if (!fiber_)
        return false;

      if (::melon_fiber_timedjoin(fiber_, NULL, timeout))
        return false;
      fiber_ = 0;
      return true;
    }

    void Fiber::detach()
    {
      if (!fiber_)
        return;
      ::melon_fiber_detach(fiber_);
      fiber_ = 0;
    }
  }
}
