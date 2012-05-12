namespace mimosa
{
  namespace container
  {
    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    Cache<Key, Value, Hash, KeyEqual>::Cache()
      : entry_timeout_(0),
        value_timeout_(0),
        lock_(),
        cleanup_thread_(nullptr),
        cache_(),
        fetch_()
    {
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    Cache<Key, Value, Hash, KeyEqual>::~Cache()
    {
      clear();
      stopCleanupThread();
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::setEntryTimeout(Time time)
    {
      entry_timeout_ = time;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::setValueTimeout(Time time)
    {
      value_timeout_ = time;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::setCleanupPeriod(Time time)
    {
      Mutex::Locker locker(cleanup_mutex_);
      cleanup_period_ = time;
      cleanup_cond_.wakeAll();
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::remove(const Key & key)
    {
      SharedMutex::Locker locker(lock_);

      cache_.erase(key);
      {
        auto it = fetch_.find(key);
        if (it != fetch_.end())
        {
          it->second->cancel();
          fetch_.erase(it);
        }
      }
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::clear()
    {
      SharedMutex::Locker locker(lock_);

      for (auto it = fetch_.begin(); it != fetch_.end(); ++it)
        it->second->cancel();

      fetch_.clear();
      cache_.clear();
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::cleanup()
    {
      SharedMutex::Locker locker(lock_);
      Time now = monotonicTime();
      for (auto it = cache_.begin(); it != cache_.end();)
        if ((entry_timeout_ > 0 &&
             it->second.last_used_ts_ + entry_timeout_ > now) ||
            (value_timeout_ > 0 &&
             it->second.value_ts_ + value_timeout_ > now))
          it = cache_.erase(it);
        else
          ++it;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::set(const Key &   key,
                                           const Value & value)
    {
      SharedMutex::Locker locker(lock_);
      Entry & entry       = cache_[key];
      entry.value_ts_     = monotonicTime();
      entry.last_used_ts_ = entry.value_ts_;
      entry.value_        = value;

      auto it = fetch_.find(key);
      if (it == fetch_.end())
        return;

      it->second->set(value);
      fetch_.erase(key);
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    typename Future<Value>::Ptr
    Cache<Key, Value, Hash, KeyEqual>::get(const Key & key)
    {
      // check read lock
      {
        SharedMutex::ReadLocker locker(lock_);
        {
          auto it = cache_.find(key);
          if (it != cache_.end())
            return new Future<Value>(it->second.value_,
                                           Future<Value>::kSet);
        }
        {
          auto it = fetch_.find(key);
          if (it != fetch_.end())
            return it->second;
        }
      }

      // check and fetch write lock
      typename Future<Value>::Ptr value;
      {
        SharedMutex::Locker locker(lock_);
        {
          auto it = cache_.find(key);
          if (it != cache_.end())
            return new Future<Value>(it->second.value_,
                                           Future<Value>::kSet);
        }
        {
          auto it = fetch_.find(key);
          if (it != fetch_.end())
            return it->second;

          value = new Future<Value>;
          fetch_[key] = value;
        }
      }
      cacheMiss(key);
      return value;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::startCleanupThread()
    {
      Mutex::Locker locker(cleanup_mutex_);
      if (cleanup_thread_)
        return;

      cleanup_thread_.reset(new Thread([this] { this->cleanupLoop(); }));
      cleanup_thread_->start();
      cleanup_thread_stop_ = false;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::stopCleanupThread()
    {
      {
        Mutex::Locker locker(cleanup_mutex_);
        if (!cleanup_thread_)
          return;

        cleanup_thread_stop_ = true;
        cleanup_cond_.wakeAll();
      }
      cleanup_thread_->join();
      cleanup_thread_ = nullptr;
    }

    template <typename Key,
              typename Value,
              typename Hash,
              typename KeyEqual>
    void
    Cache<Key, Value, Hash, KeyEqual>::cleanupLoop()
    {
      Mutex::Locker locker(cleanup_mutex_);
      while (!cleanup_thread_stop_)
      {
        if (cleanup_period_ == 0)
          cleanup_cond_.wait(cleanup_mutex_);
        else
          cleanup_cond_.timedWait(cleanup_mutex_, time() + cleanup_period_);
        cleanup();
      }
    }
  }
}
