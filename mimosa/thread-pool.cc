#include "thread-pool.hh"

namespace mimosa
{
  ThreadPool::ThreadPool(std::function<void ()> && fct)
    : stack_size_(128 * 1024),
      threads_(),
      fct_(new std::function<void ()>(fct))
  {
  }

  ThreadPool::~ThreadPool()
  {
    join();
    delete fct_;
    fct_ = nullptr;
  }

  bool
  ThreadPool::startThread()
  {
    return startThread(std::function<void ()> (*fct_));
  }

  bool
  ThreadPool::startThread(std::function<void ()> && fct)
  {
    auto thread = new Thread(std::move(fct));
    thread->setStackSize(stack_size_);
    if (!thread->start())
    {
      delete thread;
      return false;
    }

    threads_.push_back(thread);
    return true;
  }

  void
  ThreadPool::join()
  {
    for (auto & thread : threads_)
    {
      thread->join();
      delete thread;
    }
    threads_.clear();
  }
}
