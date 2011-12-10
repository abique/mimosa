namespace mimosa
{
  namespace sync
  {
    template <typename T>
    inline
    Future<T>::Future(const T & value, State state)
      : value_(value),
        state_(state),
        lock_(),
        cond_()
    {
    }

    template <typename T>
    inline
    bool
    Future<T>::isReady() const
    {
      return state_ == kSet;
    }

    template <typename T>
    inline
    bool
    Future<T>::isCanceled() const
    {
      return state_ == kCanceled;
    }

    template <typename T>
    inline
    typename Future<T>::State
    Future<T>::state() const
    {
      return state_;
    }

    template <typename T>
    inline
    void
    Future<T>::wait()
    {
      if (state_ != kUnset)
        return;

      Mutex::Locker locker(lock_);
      if (state_ != kUnset)
        return;

      cond_.wait(lock_);
    }

    template <typename T>
    inline
    T &
    Future<T>::get()
    {
      wait();
      return value_;
    }

    template <typename T>
    inline
    void
    Future<T>::set(const T & value)
    {
      Mutex::Locker locker(lock_);
      value_ = value;
      state_ = kSet;
      cond_.wakeAll();
    }

    template <typename T>
    inline
    void
    Future<T>::cancel()
    {
      Mutex::Locker locker(lock_);

      if (state_ == kSet)
        return;

      state_ = kCanceled;
      cond_.wakeAll();
    }
  }
}
