#ifndef MIMOSA_SYNC_FUTURE_HH
# define MIMOSA_SYNC_FUTURE_HH

# include "../ref-countable.hh"
# include "condition.hh"
# include "mutex.hh"

namespace mimosa
{
  namespace sync
  {
    template <typename T>
    class Future : public RefCountable<Future<T> >
    {
    public:

      enum State
      {
        kUnset,
        kSet,
        kCanceled,
      };

      inline Future(const T & value = T(), State state = kUnset);

      inline bool isReady() const;
      inline bool isCanceled() const;
      inline State state() const;
      inline void wait();
      inline T &  get();
      inline void set(const T & t);
      inline void cancel();

    private:
      T         value_;
      State     state_;
      Mutex     lock_;
      Condition cond_;
    };
  }
}

# include "future.hxx"

#endif /* !MIMOSA_SYNC_FUTURE_HH */
