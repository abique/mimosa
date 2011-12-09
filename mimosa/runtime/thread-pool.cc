#include "thread-pool.hh"

namespace mimosa
{
  namespace runtime
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
      auto thread = new Thread(std::function<void ()> (*fct_));
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
      for (auto it = threads_.begin(); it != threads_.end(); ++it)
      {
        (*it)->join();
        delete *it;
      }
      threads_.clear();
    }
  }
}
