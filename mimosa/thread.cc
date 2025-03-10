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
    (*x)();
    return nullptr;
  }

  bool
  Thread::start(std::function<void ()> && fct)
  {
    assert(state_ == kNotRunning);

    auto f = new std::function<void ()>(std::move(fct));
    if (::pthread_create(&thread_, nullptr,
                         reinterpret_cast<void*(*)(void*)>(startWrapper),
                         static_cast<void*>(f)))
    {
      delete f;
      return false;
    }

    state_ = kRunning;
    return true;
  }

  bool
  Thread::start(void *(*fct)(void *ctx), void *ctx)
  {
    assert(state_ == kNotRunning);

    if (::pthread_create(&thread_, nullptr, fct, ctx))
      return false;

    state_ = kRunning;
    return true;
  }

  Thread::Thread(Thread &&other) noexcept
    : thread_(other.thread_),
      state_(other.state_)
  {
    other.state_ = kDetached;
  }

  Thread::~Thread()
  {
    assert(state_ == kDetached || state_ == kJoined);
  }

  void Thread::join()
  {
    assert(state_ == kRunning);

    if (state_ != kRunning)
      return;

    ::pthread_join(thread_, nullptr);
    state_ = kJoined;
  }

  void Thread::detach()
  {
    assert(state_ == kRunning);

    if (state_ != kRunning)
      return;
    ::pthread_detach(thread_);
    state_ = kDetached;
  }

  void Thread::cancel()
  {
    assert(state_ == kRunning);

    if (state_ != kRunning)
      return;

    ::pthread_cancel(thread_);
  }

  void Thread::setName(const std::string &name)
  {
    assert(state_ == kRunning);

    if (state_ != kRunning)
      return;

#ifdef __WIN32__
#elif defined(__MACH__)
    if (pthread_equal(thread_, pthread_self()))
      pthread_setname_np(name.c_str());
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
