#ifndef MIMOSA_THREAD_HH
# define MIMOSA_THREAD_HH

# include <functional>
# include <stdexcept>

# include <pthread.h>

# include "non-copyable.hh"
# include "time.hh"

namespace mimosa
{
  class Thread : private NonCopyable
  {
  public:

    Thread(std::function<void ()> && fct);
    ~Thread();

    inline Thread & setStackSize(uint32_t size) { stack_size_ = size; return *this; }

    bool start();
    void join();
    bool tryJoin();
    bool timedJoin(Time timeout);
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

#endif /* !MIMOSA_THREAD_HH */
