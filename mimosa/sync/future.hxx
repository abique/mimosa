namespace mimosa
{
  namespace sync
  {
    template <typename T>
    inline
    Future<T>::Future(const T & value, bool ready)
      : value_(value),
        ready_(ready),
        lock_(),
        cond_()
    {
    }

    template <typename T>
    inline
    bool
    Future<T>::isReady()
    {
      return ready_;
    }

    template <typename T>
    inline
    void
    Future<T>::wait()
    {
      if (ready_)
        return;

      Mutex::Locker locker(lock_);
      if (ready_)
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
      assert(!ready_);
      value_ = value;
      ready_ = true;
      cond_.wakeAll();
    }
  }
}
