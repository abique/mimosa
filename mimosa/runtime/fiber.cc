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

    bool Fiber::start(std::function<void ()> && fct)
    {
      auto cb = new std::function<void ()>(fct);
      return Fiber::start(startWrapper, cb);
    }
  }
}
