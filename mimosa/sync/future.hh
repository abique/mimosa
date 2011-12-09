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

      inline Future(const T & value = T(), bool ready = false);

      inline bool isReady();
      inline void wait();
      inline T &  get();
      inline void set(const T & t);

    private:
      T         value_;
      bool      ready_;
      Mutex     lock_;
      Condition cond_;
    };
  }
}

# include "future.hxx"

#endif /* !MIMOSA_SYNC_FUTURE_HH */
