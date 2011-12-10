#ifndef MIMOSA_CONTAINER_CACHE_HH
# define MIMOSA_CONTAINER_CACHE_HH

# include <unordered_map>

# include "../runtime/time.hh"
# include "../sync/rwlock.hh"
# include "../sync/future.hh"

namespace mimosa
{
  namespace container
  {
    template <typename Key,
              typename Value,
              typename Hash = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key> >
    class Cache
    {
    public:
      typename sync::Future<Value>::Ptr get(const Key & key);

      // sets the duration after which an unsuded entry must be discarded
      void setEntryTimeout(runtime::Time time);

      // sets the duration after which the value must be discarded
      void setValueTimeout(runtime::Time time);

      void remove(const Key & key);
      void clear();

    protected:

      class Entry
      {
        runtime::Time value_ts_;
        runtime::Time last_used_ts_;
        Value         value_;
      };

      typedef std::unordered_map<Key, Entry, Hash, KeyEqual> cache_type;
      typedef std::unordered_map<Key, typename sync::Future<Value>::Ptr, Hash, KeyEqual> fetch_type;

      void set(const Key & key, Value value);
      virtual void cacheMiss(const Key & key);

      sync::RWLock cache_lock_;
      cache_type   cache_;

      sync::RWLock fecth_lock_;
      fetch_type   fetch_;
    };
  }
}

#endif /* !MIMOSA_CONTAINER_CACHE_HH */
