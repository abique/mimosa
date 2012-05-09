#ifndef MIMOSA_CONTAINER_TRIE_HH
# define MIMOSA_CONTAINER_TRIE_HH

# include "../string/string-ref.hh"

namespace mimosa
{
  namespace container
  {
    /**
     * @param Value must be a pointer type or a smart pointer
     */
    template <typename Value, string::StringRef (*GetKey)(Value value)>
    class Trie
    {
    public:
      Trie(uint32_t depth = 0);
      ~Trie();

      inline bool insert(const Value & value) { return insert(GetKey(value), value); }
      Value find(const string::StringRef & key) const;
      void erase(const string::StringRef & key);

      void clear();
      inline bool empty() const { return !size_; }
      inline uint32_t size() const { return size_; }

    private:
      // XXX
      Trie(const Trie & trie);
      Trie& operator=(const Trie & trie);

      // XXX
      Trie(Trie && trie);
      Trie& operator=(Trie && trie);

      void allocateChilds();
      bool insert(const string::StringRef & key, Value value);

      Value                  value_;
      Trie<Value, GetKey> ** childs_;
      uint32_t               depth_;
      uint32_t               size_;
    };
  }
}

#endif /* !MIMOSA_CONTAINER_TRIE_HH */
