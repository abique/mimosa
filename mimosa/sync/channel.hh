#ifndef MIMOSA_SYNC_CHANNEL_HH
# define MIMOSA_SYNC_CHANNEL_HH

# include "mutex.hh"
# include "condition.hh"

namespace mimosa
{
  namespace sync
  {
    template <typename T, typename QueueType = std::queue<T> >
    class Channel : public RefCountable<Channel>,
                    private NonCopyable
    {
    public:
      inline Channel()
        : closed_(false),
          queue_(),
          mutex_(),
          cond_()
      {
      }

      inline bool push(const T & t)
      {
        Mutex::Locker locker(mutex_);
        if (closed_)
          return false;

        queue_.push_back(t);
        cond_.wakeOne();
        return true;
      }

      inline bool pop(T & t)
      {
        Mutex::Locker locker(mutex_);
        while (true)
        {
          if (!queue_.empty())
          {
            t = queue_.front();
            queue_.pop_front();
            return true;
          }
          else if (closed_)
            return false;
          else
            cond_.wait(mutex_);
        }
      }

      inline void close()
      {
        Mutex::Locker locker(mutex_);
        closed_ = true;
        cond_.wakeAll();
      }

      bool      closed_;
      QueueType queue_;
      Mutex     mutex_;
      Condition cond_;
    };
  }
}

#endif /* !MIMOSA_SYNC_CHANNEL_HH */
