#include <pthread.h>
#include <climits>

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

    Thread::Thread(std::function<void ()> && fct)
      : thread_(),
        fct_(new std::function<void ()>(fct)),
        is_detached_(false),
        stack_size_(PTHREAD_STACK_MIN)
    {
    }

    bool
    Thread::start()
    {
      pthread_attr_t attrs;
      if (pthread_attr_init(&attrs))
        return false;

      if (pthread_attr_setstacksize(&attrs, stack_size_))
      {
        pthread_attr_destroy(&attrs);
        return false;
      }

      if (::pthread_create(&thread_, &attrs,
                           reinterpret_cast<void*(*)(void*)>(startWrapper),
                           static_cast<void*>(fct_)))
      {
        pthread_attr_destroy(&attrs);
        delete fct_;
        return false;
      }

      pthread_attr_destroy(&attrs);
      fct_ = nullptr;
      return true;
    }

    Thread::~Thread()
    {
      delete fct_;
      detach();
    }

    void Thread::join()
    {
      if (is_detached_)
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
