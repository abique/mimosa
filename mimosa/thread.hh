#pragma once

#include <functional>
#include <stdexcept>

#include <pthread.h>

#include "non-copyable.hh"
#include "non-movable.hh"
#include "time.hh"

namespace mimosa
{
  class Thread : private NonCopyable, private NonMovable
  {
  public:

    Thread(std::function<void ()> && fct);
    ~Thread();

    inline Thread & setStackSize(uint32_t size) { stack_size_ = size; return *this; }
    void setName(const std::string &name);
    static void setCurrentName(const std::string &name);

    bool start();
    void join();
    void detach();
    void cancel();

    inline pthread_t threadId() const { return thread_; }

  private:

    enum State
    {
      kNotRunning,
      kRunning,
      kDetached,
      kJoined,
    };

    pthread_t                thread_;
    std::function<void ()> * fct_;
    State                    state_;
    uint32_t                 stack_size_;
  };
}
