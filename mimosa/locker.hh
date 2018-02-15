#pragma once

#include "non-copyable.hh"
#include "non-movable.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  template <typename T>
  class Locker : private NonCopyable, private NonMovable
  {
  public:
    inline explicit Locker(T & mutex) : mutex_(mutex) { mutex_.lock(); }
    inline ~Locker() { mutex_.unlock(); }
  private:
    T & mutex_;
  };
}

