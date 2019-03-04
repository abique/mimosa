#pragma once

#include <list>
#include <thread>
#include <functional>

namespace mimosa
{
  class ThreadPool
  {
  public:
    ThreadPool(std::function<void ()> && fct);
    ~ThreadPool();

    void startThread();
    void startThreads(uint32_t nb);

    void join();

  private:
    const std::function<void ()> fct_;
    std::list<std::thread>       threads_;
  };
}
