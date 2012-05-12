#ifndef MIMOSA_SHARED_LOCKER_HH
# define MIMOSA_SHARED_LOCKER_HH

# include "non-copyable.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  template <typename T>
  class SharedLocker : private NonCopyable
  {
  public:

    inline SharedLocker(T & rwlock)
    : rwlock_(rwlock)
    {
      rwlock_.sharedLock();
    }

    inline ~SharedLocker()
    {
      rwlock_.unlock();
    }

  private:
    T & rwlock_;
  };
}

#endif /* !MIMOSA_SHARED_LOCKER_HH */
