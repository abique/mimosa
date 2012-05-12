#ifndef MIMOSA_CONTAINER_TRIE_HH
# define MIMOSA_CONTAINER_TRIE_HH

# include "../string-ref.hh"

namespace mimosa
{
  namespace container
  {
    /**
     * @param Value must be a pointer type or a smart pointer
     */
    template <typename Value, StringRef (*GetKey)(Value value)>
    class Trie
    {
    public:
      inline Trie(uint32_t depth = 0);
      inline ~Trie();

      inline bool insert(const Value & value) { return insert(GetKey(value), value); }
      inline Value find(const StringRef & key) const;
      inline void erase(const StringRef & key);

      inline void clear();
      inline bool empty() const { return !size_; }
      inline uint32_t size() const { return size_; }

    private:
      // XXX
      Trie(const Trie & trie);
      Trie& operator=(const Trie & trie);

      // XXX
      Trie(Trie && trie);
      Trie& operator=(Trie && trie);

      inline void allocateChilds();
      inline bool insert(const StringRef & key, Value value);

      Value                  value_;
      Trie<Value, GetKey> ** childs_;
      uint32_t               depth_;
      uint32_t               size_;
    };
  }
}

# include "trie.hxx"

#endif /* !MIMOSA_CONTAINER_TRIE_HH */
