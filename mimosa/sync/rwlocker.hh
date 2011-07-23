#ifndef MIMOSA_SYNC_RWLOCKER_HH
# define MIMOSA_SYNC_RWLOCKER_HH

# include "../non-copyable.hh"

namespace mimosa
{
  namespace sync
  {
    template <typename T>
    class RWLocker : private NonCopyable
    {
    public:

      enum State
      {
        kUnlocked,
        kReadLocked,
        kWriteLocked
      };

      inline RWLocker(T & rwlock, State acquire = kReadLocked)
        : state_(acquire), rwlock_(rwlock)
      {
        if (acquire == kReadLocked)
          rwlock_.rdlock();
        else if (acquire == kWriteLocked)
          rwlock_.wrlock();
      }

      inline ~RWLocker()
      {
        if (state_ != kUnlocked)
          rwlock_.unlock();
      }

    private:
      State state_;
      T &   rwlock_;
    };
  }
}

#endif /* !MIMOSA_SYNC_RWLOCKER_HH */
