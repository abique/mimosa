#ifndef MIMOSA_CHANNEL_HH
# define MIMOSA_CHANNEL_HH

# include <queue>
# include <limits>

# include "mutex.hh"
# include "condition.hh"
# include "ref-countable.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  template <typename T, typename QueueType = std::queue<T> >
  class Channel : public RefCountable<Channel<T, QueueType> >,
                  private NonCopyable
  {
  public:
    inline Channel()
      : closed_(false),
        queue_(),
        mutex_(),
        cond_(),
        max_size_(std::numeric_limits<decltype (max_size_)>::max())
    {
    }

    inline void setMaxSize(size_t max_size)
    {
      Mutex::Locker locker(mutex_);
      if (max_size_ < max_size &&
          max_size_ <= queue_.size() && queue_.size() < max_size)
        push_cond_.wakeAll();
      max_size_ = max_size;
    }

    inline bool empty() const
    {
      return queue_.empty();
    }

    inline size_t maxSize() const
    {
      return max_size_;
    }

    inline bool push(const T & t)
    {
      Mutex::Locker locker(mutex_);
      do {
        if (closed_)
          return false;

        if (queue_.size() < max_size_)
          break;
        push_cond_.wait(mutex_);
      } while (true);

      queue_.push(t);
      cond_.wakeOne();
      return true;
    }

    inline bool tryPush(const T & t)
    {
      Mutex::Locker locker(mutex_);
      if (closed_)
        return false;

      if (queue_.size() >= max_size_)
        return false;

      queue_.push(t);
      cond_.wakeOne();
      return true;
    }

    inline bool push(T && t)
    {
      Mutex::Locker locker(mutex_);
      do {
        if (closed_)
          return false;

        if (queue_.size() < max_size_)
          break;
        push_cond_.wait(mutex_);
      } while (true);

      queue_.push(std::move(t));
      cond_.wakeOne();
      return true;
    }

    inline bool tryPush(T && t)
    {
      Mutex::Locker locker(mutex_);
      if (closed_)
        return false;

      if (queue_.size() >= max_size_)
        return false;

      queue_.push(std::move(t));
      cond_.wakeOne();
      return true;
    }

    inline bool pop(T & t)
    {
      Mutex::Locker locker(mutex_);
      while (true) {
        if (!queue_.empty())
        {
          t = std::move(queue_.front());
          queue_.pop();
          push_cond_.wakeOne();
          return true;
        }
        else if (closed_)
          return false;
        cond_.wait(mutex_);
      }
      return false;
    }

    inline bool tryPop(T & t)
    {
      Mutex::Locker locker(mutex_);
      if (queue_.empty())
        return false;

      t = std::move(queue_.front());
      queue_.pop();
      push_cond_.wakeOne();
      return true;
    }

    /** closes the push end, it is possible to pop
     * until the channel is empty */
    inline void close()
    {
      Mutex::Locker locker(mutex_);
      closed_ = true;
      cond_.wakeAll();
      push_cond_.wakeAll();
    }

  private:
    bool      closed_;
    QueueType queue_;
    Mutex     mutex_;
    Condition cond_;
    Condition push_cond_;
    size_t    max_size_;
  };
}

#endif /* !MIMOSA_CHANNEL_HH */
