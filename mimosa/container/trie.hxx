namespace mimosa
{
  namespace container
  {
    template <typename Value, StringRef (*GetKey)(Value value)>
    inline
    Trie<Value, GetKey>::Trie(uint32_t depth)
      : value_(),
        childs_(nullptr),
        depth_(depth),
        size_(0)
    {
    }

    template <typename Value, StringRef (*GetKey)(Value value)>
    inline
    Trie<Value, GetKey>::~Trie()
    {
      clear();
    }

    template <typename Value, StringRef (*GetKey)(Value value)>
    inline void
    Trie<Value, GetKey>::clear()
    {
      if (childs_) {
        for (int i = 0; i < 256; ++i)
          delete childs_[i];
        free(childs_);
        childs_ = nullptr;
      }
    }

    template <typename Value, StringRef (*GetKey)(Value value)>
    inline void
    Trie<Value, GetKey>::allocateChilds()
    {
      assert(!childs_);
      childs_ = (Trie<Value, GetKey>**)calloc(sizeof (*childs_), 256);

      // check if value_ should go to childs_
      if (!value_)
        return;

      StringRef key = GetKey(value_);
      if (key.size() == depth_)
        return;

      insert(key, value_);
      value_ = nullptr;
      --size_;
    }

    template <typename Value, StringRef (*GetKey)(Value value)>
    inline bool
    Trie<Value, GetKey>::insert(const StringRef & key, Value value)
    {
      assert(key.size() >= depth_);

      if (key.size() == depth_) {
        if (!value_) {
          value_ = value;
          ++size_;
          return true;
        }

        if (GetKey(value_).size() > depth_) {
          std::swap(value, value_);
          insert(value);
          return true;
        }

        return false;
      }

      if (!value_ && !childs_) {
        value_ = value;
        ++size_;
        return true;
      }

      if (!childs_)
        allocateChilds();

      if (!childs_[(uint8_t)key[depth_]])
        childs_[(uint8_t)key[depth_]] = new Trie<Value, GetKey>(depth_ + 1);
      if (!childs_[(uint8_t)key[depth_]]->insert(key, value))
        return false;
      ++size_;
      return true;
    }

    template <typename Value, StringRef (*GetKey)(Value value)>
    inline Value
    Trie<Value, GetKey>::find(const StringRef & key) const
    {
      assert(key.size() >= depth_);

      if (key.size() == depth_) {
        if (value_)
          return value_;
        return nullptr;
      }

      if (!childs_ || !childs_[(uint8_t)key[depth_]])
        return nullptr;

      return childs_[(uint8_t)key[depth_]]->find(key);
    }

    template <typename Value, StringRef (*GetKey)(Value value)>
    inline void
    Trie<Value, GetKey>::erase(const StringRef & key)
    {
      if (key.size() == depth_) {
        value_ = nullptr;
        --size_;
        return;
      }

      if (!childs_ || !childs_[(uint8_t)key[depth_]])
        return;

      childs_[(uint8_t)key[depth_]]->erase(key);
      --size_;

      if (childs_[(uint8_t)key[depth_]]->empty()) {
        delete childs_[(uint8_t)key[depth_]];
        childs_[(uint8_t)key[depth_]] = nullptr;
      }
    }
  }
}
