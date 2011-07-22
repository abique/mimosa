#ifndef MIMOSA_RWLOCK_HH
# define MIMOSA_RWLOCK_HH

namespace mimosa
{
  class RwLock : public NonCopyable
  {
  public:
    typedef RwLocker<RwLock> Locker;

    inline RwLock() : lock_(::melon_rwlock_new()) {}
    inline ~RwLock() { ::melon_mutex_destroy(lock_); }
    inline bool ok() const { return lock_; }
    inline void readLock() { ::melon_rwlock_rdlock(lock_); }
    inline void tryReadLock() { ::melon_rwlock_tryrdlock(lock_); }
    inline bool timedReadLock(::melon_time_t time) { return ::melon_rwlock_timedrdlock(lock_, time); }
    inline void writelock() { ::melon_rwlock_wrlock(lock_); }
    inline void tryWriteLock() { ::melon_rwlock_trywrlock(lock_); }
    inline bool timedWritelock(::melon_time_t time) { return ::melon_rwlock_timedwrlock(lock_, time); }
    inline void unlock() { ::melon_rwlock_unlock(lock_); }
  private:
    ::melon_rwlock * lock_;
  };
}

#endif /* !MIMOSA_RWLOCK_HH */
