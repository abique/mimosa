#include "thread-pool.hh"

namespace mimosa
{
  ThreadPool::ThreadPool(std::function<void ()> && fct)
    : fct_(std::move(fct)),
      threads_()
  {
  }

  ThreadPool::~ThreadPool()
  {
    join();
  }

  void ThreadPool::startThread()
  {
    threads_.emplace_back(fct_);
  }

  void ThreadPool::startThreads(uint32_t nb)
  {
    for (uint32_t i = 0; i < nb; ++i)
      startThread();
  }

  void
  ThreadPool::join()
  {
    for (auto & thread : threads_)
      thread.join();
    threads_.clear();
  }
}
