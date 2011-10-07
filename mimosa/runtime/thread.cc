#include "thread.hh"

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

    void Thread::start(std::function<void ()> && fct)
    {
      auto cb = new std::function<void ()>(fct);
      Thread::start(startWrapper, cb);
    }

    Thread::Thread(std::function<void ()> && fct)
      : thread_(),
        is_detached_(false)
    {
      auto cb = new std::function<void ()>(fct);
      if (::pthread_create(&thread_, nullptr,
                               reinterpret_cast<void*(*)(void*)>(startWrapper),
                               static_cast<void*>(cb)))
      {
        delete cb;
        throw std::runtime_error("failed to start new fiber");
      }
    }

    Thread::~Thread()
    {
      detach();
    }

    void Thread::join()
    {
      if (!is_detached_)
        return;
      ::pthread_join(thread_, NULL);
      is_detached_ = true;
    }

    bool Thread::tryJoin()
    {
      if (is_detached_)
        return false;

      if (::pthread_tryjoin_np(thread_, NULL))
        return false;
      is_detached_ = true;
      return true;
    }

    bool Thread::timedJoin(Time timeout)
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

    void Thread::detach()
    {
      if (is_detached_)
        return;
      ::pthread_detach(thread_);
    }
  }
}
