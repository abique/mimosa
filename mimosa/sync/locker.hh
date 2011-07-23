#ifndef MIMOSA_SYNC_LOCKER_HH
# define MIMOSA_SYNC_LOCKER_HH

# include "../non-copyable.hh"

namespace mimosa
{
  namespace sync
  {
    template <typename T>
    class Locker : private NonCopyable
    {
    public:
      inline Locker(T & mutex) : mutex_(mutex) { mutex.lock(); }
      inline ~Locker() { mutex.unlock(); }
    private:
      T & mutex_;
    };
  }
}

#endif /* !MIMOSA_SYNC_LOCKER_HH */
