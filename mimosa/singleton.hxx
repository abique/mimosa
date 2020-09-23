namespace mimosa
{
  template <typename T>
  std::atomic<T *> Singleton<T>::instance_ = {nullptr};

  template <typename T>
  T &
  Singleton<T>::instance()
  {
    if (!instance_) {
      T *t = new T;
      T *n = nullptr;
      if (!instance_.compare_exchange_weak(n, t))
        delete t;
    }
    return *instance_;
  }

  template <typename T>
  void
  Singleton<T>::release()
  {
    T *t = instance_;
    if (instance_.compare_exchange_strong(t, nullptr))
      delete t;
  }
}
