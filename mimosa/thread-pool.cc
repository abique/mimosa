#include "thread-pool.hh"

namespace mimosa
{
  ThreadPool::ThreadPool(std::function<void ()> && fct)
    : fct_(std::make_unique<std::function<void ()>>(std::move(fct)))
  {
  }

  ThreadPool::~ThreadPool()
  {
    join();
  }

  bool
  ThreadPool::startThread()
  {
    return startThread(std::function<void ()> (*fct_));
  }

  bool
  ThreadPool::startThread(std::function<void ()> && fct)
  {
    Thread t;
    if (!t.start(std::move(fct)))
      return false;

    threads_.emplace_back(std::move(t));
    return true;
  }

  void
  ThreadPool::join()
  {
    for (auto & thread : threads_)
      thread.join();
    threads_.clear();
  }
}
