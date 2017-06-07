#pragma once

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

