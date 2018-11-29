#include <pthread.h>
#include <climits>
#include <cassert>
#include <memory>

#include <string>

#include "thread.hh"

namespace mimosa
{
  static void* startWrapper(std::function<void ()> * fct)
  {
    std::unique_ptr<std::function<void ()> > x(fct);
    (*fct)();
    return nullptr;
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

    ::pthread_join(thread_, nullptr);
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

  void Thread::setName(const std::string &name)
  {
#ifdef __WIN32__
#elif defined(__MACH__)
#elif defined(__HAIKU__)
#else
    pthread_setname_np(thread_, name.c_str());
#endif
  }

  void Thread::setCurrentName(const std::string &name)
  {
#ifdef __MACH__
    pthread_setname_np(name.c_str());
#elif defined(__WIN32__)
#elif defined(__HAIKU__)
#else
    pthread_setname_np(pthread_self(), name.c_str());
#endif
  }
}
