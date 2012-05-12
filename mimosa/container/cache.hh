#ifndef MIMOSA_CONTAINER_CACHE_HH
# define MIMOSA_CONTAINER_CACHE_HH

# include <memory>
# include <unordered_map>

# include "../time.hh"
# include "../thread.hh"
# include "../shared-mutex.hh"
# include "../future.hh"

namespace mimosa
{
  namespace container
  {
    template <typename Key,
              typename Value,
              typename Hash = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key> >
    class Cache : private NonCopyable
    {
    public:
      Cache();
      ~Cache();

      typename Future<Value>::Ptr get(const Key & key);

      /// sets the duration after which an unsuded entry must be discarded
      void setEntryTimeout(Time time);

      /// sets the duration after which the value must be discarded
      void setValueTimeout(Time time);

      /// sets the cleanup period
      void setCleanupPeriod(Time time);

      /// starts the cleanup thread
      void startCleanupThread();

      /// stops the cleanup thread
      void stopCleanupThread();

      /// removes the specific key from the cache
      void remove(const Key & key);
      /// cleans up old cache entries
      void cleanup();
      /// removes every entries from the cache and cancels fetchs
      void clear();

    protected:

      class Entry
      {
      public:
        Time value_ts_;
        Time last_used_ts_;
        Value         value_;
      };

      typedef std::unordered_map<Key, Entry, Hash, KeyEqual> cache_type;
      typedef std::unordered_map<Key, typename Future<Value>::Ptr, Hash, KeyEqual> fetch_type;

      void set(const Key & key, const Value & value);
      virtual void cacheMiss(const Key & key) = 0;
      void cleanupLoop();

      Time entry_timeout_;
      Time value_timeout_;
      Time cleanup_period_;

      SharedMutex lock_;

      bool                    cleanup_thread_stop_;
      std::unique_ptr<Thread> cleanup_thread_;
      Mutex                   cleanup_mutex_;
      Condition               cleanup_cond_;

      cache_type cache_;
      fetch_type fetch_;
    };
  }
}

#endif /* !MIMOSA_CONTAINER_CACHE_HH */
