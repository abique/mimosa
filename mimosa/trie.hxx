#pragma once

#include "trie.hh"

namespace mimosa {
   template <typename Value, StringRef (*GetKey)(Value value)>
   inline Trie<Value, GetKey>::Trie(uint32_t depth) : value_(nullptr), depth_(depth) {}

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline Trie<Value, GetKey>::~Trie() {
      clear();
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline void Trie<Value, GetKey>::clear() {
      childs_.reset();
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline void Trie<Value, GetKey>::allocateChilds() {
      assert(!childs_);
      childs_.reset(new childs_array_type);

      // check if value_ should go to childs_
      if (!value_)
         return;

      StringRef key = GetKey(value_);
      if (key.size() == depth_)
         return;

      auto child = new Trie<Value, GetKey>(depth_ + 1);
      setChild(key[depth_], child);
      bool ret = child->insert(key, value_);
      value_ = nullptr;
      assert(ret);
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline bool Trie<Value, GetKey>::insert(const StringRef &key, Value value) {
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

      auto child = getOrCreateChild(key[depth_]);
      if (!child->insert(key, value))
         return false;
      ++size_;
      return true;
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline Trie<Value, GetKey> *Trie<Value, GetKey>::prefix(const StringRef &key) const {
      assert(key.size() >= depth_);

      if (key.size() == depth_)
         return this;

      if (size_ == 1) {
         if (value_ && GetKey(value_) == key)
            return this;
         return nullptr;
      }

      auto child = getChild(key[depth_]);
      if (!child)
         return nullptr;

      return child->prefix(key);
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline Value Trie<Value, GetKey>::find(const StringRef &key) const {
      assert(key.size() >= depth_);

      if (key.size() == depth_ || size_ == 1) {
         if (value_ && GetKey(value_) == key)
            return value_;
         return nullptr;
      }

      auto child = getChild(key[depth_]);
      if (!child)
         return nullptr;

      return child->find(key);
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline bool Trie<Value, GetKey>::erase(const StringRef &key) {
      // check if value matches the key
      if ((key.size() == depth_ || size_ == 1) && (value_ && GetKey(value_) == key)) {
         value_ = nullptr;
         --size_;
         repack();
         return true;
      }

      // check if we can find the key in childs
      const uint8_t k = key[depth_];
      auto child = getChild(k);
      if (!child)
         return false;

      if (!child->erase(key))
         return false; // childs_ did not holds key

      --size_;
      if (child->empty())
         eraseChild(k);

      repack();
      return true;
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline void Trie<Value, GetKey>::repack() {
      // can we free childs and move our last value to value_?
      if (size_ != 1 || value_ || !childs_)
         return;

      for (auto &child : (*childs_))
         if (child && child->value_) {
            value_ = child->value_;
            childs_.reset();
            return;
         }
   }

   template <typename Value, StringRef (*GetKey)(Value value)>
   inline void Trie<Value, GetKey>::foreach (const std::function<void(Value)> &callback) const {
      if (value_)
         callback(value_);

      if (!childs_)
         return;

      for (auto &child : (*childs_))
         if (child)
            child->foreach (callback);
   }
} // namespace mimosa
