#ifndef MIMOSA_THREAD_POOL_HH
# define MIMOSA_THREAD_POOL_HH

# include <list>

# include "thread.hh"

namespace mimosa
{
  class ThreadPool
  {
  public:
    ThreadPool(std::function<void ()> && fct);
    ~ThreadPool();

    inline ThreadPool & setStackSize(uint32_t size) { stack_size_ = size; return *this; }

    bool startThread();

    void join();

  private:
    uint32_t                 stack_size_;
    std::list<Thread *>      threads_;
    std::function<void ()> * fct_;
  };
}

#endif /* !MIMOSA_THREAD_POOL_HH */
