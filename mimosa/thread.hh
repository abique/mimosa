#pragma once

#include <functional>
#include <stdexcept>

#include <pthread.h>

#include "non-copyable.hh"
#include "non-movable.hh"
#include "time.hh"

namespace mimosa
{
  class Thread
  {
  public:
    constexpr Thread() = default;
    Thread(const Thread& other) = delete;
    Thread(Thread && other) noexcept;
    ~Thread();

    Thread& operator=(const Thread& other) = delete;
    Thread& operator=(Thread&& other) = delete;

    void setName(const std::string &name);
    static void setCurrentName(const std::string &name);

    bool start(std::function<void ()> && fct);
    bool start(void *(*fct)(void *ctx), void *ctx);

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

    pthread_t thread_ = 0;
    State     state_ = kNotRunning;
  };
}
