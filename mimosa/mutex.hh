#ifndef MIMOSA_MUTEX_HH
# define MIMOSA_MUTEX_HH

# include <melon/melon.h>

# include "non-copyable.hh"

namespace mimosa
{
  class Mutex : private NonCopyable
  {
  public:
    typedef Locker<Mutex> Locker;
    typedef UniqueLocker<Mutex> UniqueLocker;

    inline Mutex() : mutex_(::melon_mutex_new()) {}
    inline ~Mutex() { ::melon_mutex_destroy(mutex_); }
    inline bool ok() const { return mutex_; }
    inline void lock() { ::melon_mutex_lock(mutex_); }
    inline void unlock() { ::melon_mutex_lock(mutex_); }
    inline bool tryLock() { return ::melon_mutex_trylock(mutex_); }
    inline bool timedLock(::melon_time_t time) { return ::melon_mutex_timedlock(mutex_, time); }
  private:
    friend class Condition;
    ::melon_mutex * mutex_;
  };
}

#endif /* !MIMOSA_MUTEX_HH */
