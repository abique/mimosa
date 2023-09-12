#pragma once

#include <array>
#include <functional>
#include <memory>

#include "string-ref.hh"

namespace mimosa {
   /**
    * @param Value must be a pointer type or a smart pointer
    */
   template <typename Value, StringRef (*GetKey)(Value value)>
   class Trie {
   public:
      inline Trie(uint32_t depth = 0);
      inline ~Trie();

      inline bool insert(const Value &value) { return insert(GetKey(value), value); }

      /**
       * @return a valid pointer if found, nullptr otherwise
       */
      inline Value find(const StringRef &key) const;

      /**
       * @return a valid pointer if found, nullptr otherwise
       */
      inline Trie<Value, GetKey> *prefix(const StringRef &key) const;

      /**
       * @return true if one element has been removed, false otherwise
       */
      inline bool erase(const StringRef &key);

      /**
       * @return true if one element has been removed, false otherwise
       */
      inline bool erase(const Value &value) { return erase(GetKey(value)); }

      inline void clear();
      inline bool empty() const { return !size_; }
      inline uint32_t size() const { return size_; }

      inline void foreach (const std::function<void(Value)> &callback) const;

   private:
      // XXX
      Trie(const Trie &trie) = delete;
      Trie &operator=(const Trie &trie) = delete;

      // XXX
      Trie(Trie &&trie) = delete;
      Trie &operator=(Trie &&trie) = delete;

      inline void allocateChilds();
      inline bool insert(const StringRef &key, Value value);
      inline void repack();

      inline Trie<Value, GetKey> *getChild(uint8_t index) const noexcept {
         return childs_ ? (*childs_)[index].get() : nullptr;
      }

      inline Trie<Value, GetKey> *getOrCreateChild(uint8_t index) {
         assert(childs_);
         auto child = (*childs_)[index].get();
         if (child)
            return child;

         child = new Trie<Value, GetKey>(depth_ + 1);
         setChild(index, child);
         return child;
      }

      inline void setChild(uint8_t index, Trie<Value, GetKey> *child) noexcept {
         assert(childs_);
         (*childs_)[index].reset(child);
      }

      inline void eraseChild(uint8_t index) noexcept {
         assert(childs_);
         (*childs_)[index].reset();
      }

      using childs_array_type = std::array<std::unique_ptr<Trie<Value, GetKey>>, 256>;

      Value value_;
      std::unique_ptr<childs_array_type> childs_;
      uint32_t depth_ = 0;
      uint32_t size_ = 0;
   };
} // namespace mimosa
