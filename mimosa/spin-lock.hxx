#pragma once

namespace mimosa
{
  inline
  SpinLock::~SpinLock() noexcept
  {
    /* wait for unlock */
    while (lock_)
      ;
  }

  inline void
  SpinLock::lock() noexcept
  {
    while (!__sync_bool_compare_and_swap(&lock_, 0, 1))
      ;
  }

  inline bool
  SpinLock::tryLock() noexcept
  {
    return __sync_bool_compare_and_swap(&lock_, 0, 1);
  }

  inline void
  SpinLock::unlock() noexcept
  {
    lock_ = 0;
  }
}
