#pragma once

#include <vector>

#include "thread.hh"

namespace mimosa
{
  class ThreadPool
  {
  public:
    explicit ThreadPool(std::function<void ()> && fct);
    ~ThreadPool();

    inline ThreadPool & setStackSize(uint32_t size) { stack_size_ = size; return *this; }

    bool startThread();
    inline uint32_t startThreads(uint32_t nb) {
      for (uint32_t i = 0; i < nb; ++i)
        if (!startThread())
          return i;
      return nb;
    }

    bool startThread(std::function<void ()> && fct);

    void join();

  private:
    uint32_t                 stack_size_;
    std::vector<Thread>      threads_;
    std::function<void ()> * fct_;
  };
}
