namespace mimosa
{
  template <typename Value, StringRef (*GetKey)(Value value)>
  inline
  Trie<Value, GetKey>::Trie(uint32_t depth)
    : value_(nullptr),
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

    childs_[(uint8_t)key[depth_]] = new Trie<Value, GetKey>(depth_ + 1);
    bool ret = childs_[(uint8_t)key[depth_]]->insert(key, value_);
    value_ = nullptr;
    assert(ret);
  }

  template <typename Value, StringRef (*GetKey)(Value value)>
  inline bool
  Trie<Value, GetKey>::insert(const StringRef & key, Value value)
  {
    assert(key.size() >= depth_);

    if (empty()) {
      assert(!value_);
      assert(!childs_);
      value_ = value;
      ++size_;
      return true;
    }

    // don't add two times the same key
    if (size_ == 1 && GetKey(value_) == key)
      return false;

    // this node equals the key
    if (key.size() == depth_) {
      if (!value_) {
        value_ = value;
        ++size_;
        return true;
      }

      if (GetKey(value_).size() > depth_) {
        std::swap(value, value_);
        bool ret = insert(value);
        assert(ret);
        return ret;
      }

      assert(key == GetKey(value_));
      return false;
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
  inline Trie<Value, GetKey> *
  Trie<Value, GetKey>::prefix(const StringRef & key) const
  {
    assert(key.size() >= depth_);

    if (key.size() == depth_)
      return this;

    if (size_ == 1) {
      if (value_ && GetKey(value_) == key)
        return this;
      return nullptr;
    }

    if (!childs_ || !childs_[(uint8_t)key[depth_]])
      return nullptr;

    return childs_[(uint8_t)key[depth_]]->prefix(key);
  }

  template <typename Value, StringRef (*GetKey)(Value value)>
  inline Value
  Trie<Value, GetKey>::find(const StringRef & key) const
  {
    assert(key.size() >= depth_);

    if (key.size() == depth_ || size_ == 1) {
      if (value_ && GetKey(value_) == key)
        return value_;
      return nullptr;
    }

    if (!childs_ || !childs_[(uint8_t)key[depth_]])
      return nullptr;

    return childs_[(uint8_t)key[depth_]]->find(key);
  }

  template <typename Value, StringRef (*GetKey)(Value value)>
  inline bool
  Trie<Value, GetKey>::erase(const StringRef & key)
  {
    // check if value matches the key
    if ((key.size() == depth_ || size_ == 1) &&
        (value_ && GetKey(value_) == key))
    {
      value_ = nullptr;
      --size_;
      repack();
      return true;
    }

    // check if we can find the key in childs
    if (!childs_ || !childs_[(uint8_t)key[depth_]])
      return false;

    if (!childs_[(uint8_t)key[depth_]]->erase(key))
      return false; // childs_ did not holds key

    --size_;
    if (childs_[(uint8_t)key[depth_]]->empty()) {
      delete childs_[(uint8_t)key[depth_]];
      childs_[(uint8_t)key[depth_]] = nullptr;
    }
    repack();
    return true;
  }

  template <typename Value, StringRef (*GetKey)(Value value)>
  inline void
  Trie<Value, GetKey>::repack()
  {
    // can we free childs and move our last value to value_?
    if (size_ != 1 || value_ || !childs_)
      return;

    for (int i = 0; i < 256; ++i)
      if (childs_[i] && childs_[i]->value_)
      {
        value_ = childs_[i]->value_;
        delete childs_[i];
        free(childs_);
        childs_ = nullptr;
        return;
      }
  }

  template <typename Value, StringRef (*GetKey)(Value value)>
  inline void
  Trie<Value, GetKey>::foreach(const std::function<void (Value)> & callback) const
  {
    if (value_)
      callback(value_);

    if (childs_)
      for (int i = 0; i < 256; ++i)
        if (childs_[i])
          childs_[i]->foreach(callback);
  }
}
