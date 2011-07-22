#ifndef RWLOCKER_HH
# define RWLOCKER_HH

namespace mimosa
{
  template <typename T>
  class RwLocker : private NonCopyable
  {
  public:
    enum State { kUnlocked, kReadLocked, kWriteLocked };
    inline RwLocker(T & lock, State acquire = kWriteLocked)
      : state_(acquire), lock_(lock)
    {
      if (acquire == kReadLocked)
        lock_.rdlock();
      else if (acquire == kWriteLocked)
        lock_.wrlock();
    }
    inline ~RwLocker() { if (state_ != kUnlocked) lock_.unlock(); }
  private:
    State  state_;
    RwLock lock_;
  };
}

#endif /* !RWLOCKER_HH */
