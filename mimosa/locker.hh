#ifndef MIMOSA_LOCKER_HH
# define MIMOSA_LOCKER_HH

namespace mimosa
{
  template <typename T>
  class Locker : private NonCopyable
  {
  public:
    inline ScopedLocker(T & mutex) : mutex_(mutex) { mutex.lock(); }
    inline ~ScopedLocker() { mutex.unlock(); }
  private:
    T & mutex_;
  };
}

#endif /* !MIMOSA_LOCKER_HH */
