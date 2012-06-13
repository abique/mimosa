#ifndef MIMOSA_TRIE_HH
# define MIMOSA_TRIE_HH

# include <functional>

# include "string-ref.hh"

namespace mimosa
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

    /**
     * @return a valid pointer if found, nullptr otherwise
     */
    inline Value find(const StringRef & key) const;

    /**
     * @return a valid pointer if found, nullptr otherwise
     */
    inline Trie<Value, GetKey> * prefix(const StringRef & key) const;

    /**
     * @return true if one element has been removed, false otherwise
     */
    inline bool erase(const StringRef & key);

    /**
     * @return true if one element has been removed, false otherwise
     */
    inline bool erase(const Value & value) { return erase(GetKey(value)); }

    inline void clear();
    inline bool empty() const { return !size_; }
    inline uint32_t size() const { return size_; }

    inline void foreach(const std::function<void (Value)> & callback) const;

  private:
    // XXX
    Trie(const Trie & trie);
    Trie& operator=(const Trie & trie);

    // XXX
    Trie(Trie && trie);
    Trie& operator=(Trie && trie);

    inline void allocateChilds();
    inline bool insert(const StringRef & key, Value value);
    inline void repack();

    Value                  value_;
    Trie<Value, GetKey> ** childs_;
    uint32_t               depth_;
    uint32_t               size_;
  };
}

# include "trie.hxx"

#endif /* !MIMOSA_TRIE_HH */
