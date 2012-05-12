namespace mimosa
{
  inline
  SpinLock::SpinLock()
    : lock_(0)
  {
  }

  inline
  SpinLock::~SpinLock()
  {
    /* wait for unlock */
    while (lock_)
      ;
  }

  inline void
  SpinLock::lock()
  {
    while (!__sync_bool_compare_and_swap(&lock_, 0, 1))
      ;
  }

  inline bool
  SpinLock::tryLock()
  {
    return __sync_bool_compare_and_swap(&lock_, 0, 1);
  }

  inline void
  SpinLock::unlock()
  {
    lock_ = 0;
  }
}
