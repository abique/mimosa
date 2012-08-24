#include <pthread.h>
#include <climits>
#include <cassert>

#include "thread.hh"

namespace mimosa
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
      state_(kNotRunning),
      stack_size_(64 * 1024)
      //stack_size_(PTHREAD_STACK_MIN)
  {
  }

  bool
  Thread::start()
  {
    assert(state_ == kNotRunning);

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
      return false;
    }

    pthread_attr_destroy(&attrs);
    fct_ = nullptr;
    state_ = kRunning;
    return true;
  }

  Thread::~Thread()
  {
    delete fct_;
    detach();
  }

  void Thread::join()
  {
    if (state_ != kRunning)
      return;

    ::pthread_join(thread_, NULL);
    state_ = kJoined;
  }

  void Thread::detach()
  {
    if (state_ != kRunning)
      return;
    ::pthread_detach(thread_);
    state_ = kDetached;
  }

  void Thread::cancel()
  {
    if (state_ != kRunning)
      return;
    ::pthread_cancel(thread_);
  }
}
