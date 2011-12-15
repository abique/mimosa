namespace mimosa
{
  namespace container
  {
    template <typename T>
    T * Singleton<T>::instance_ = nullptr;

    template <typename T>
    T &
    Singleton<T>::instance()
    {
      if (!instance_)
        instance_ = new T;
      return *instance_;
    }

    template <typename T>
    void
    Singleton<T>::release()
    {
      delete instance_;
      instance_ = nullptr;
    }
  }
}
