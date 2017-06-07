#pragma once

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

# include <string>
# include <cstdint>
# include <cassert>

namespace mimosa
{
  namespace flat
  {
    /**
     * @class Flat
     *
     * This class is an helper to manage memory mapped.
     * It is usefull when dealing with custom data structures mapped from
     * the disk or on memory.
     *
     * @warning Flat rely on mremap() for brk() sbrk() and reserve(), so
     * pointers obtained from base(), ptr() and ptr0() are valid until a
     * mremap operation happened.
     */
    class Flat
    {
    public:
      /**
       * This creates a flat memory mapping
       *
       * @param path the path to the file to be mapped, if path is empty
       * then we will map RAM.
       * @param oflags the open flags, see man 2 open
       * @param mode the open mode, see
       */
      Flat(const std::string & path, int oflags, mode_t mode);
      ~Flat();

      /**
       * checks that the flat mapping succeed
       */
      inline bool ok() const { return base_; }

      /**
       * @return the used size
       */
      inline size_t size() const { return size_; }

      /**
       * @return the mapped size
       */
      inline size_t mappedSize() const { return mapped_size_; }

      /**
       * convient helper to know the base pointer of the memory mapping
       */
      inline void * base() const { return base_; }

      /**
       * Ensure that we have at least size mapped
       */
      bool reserve(size_t size);

      /**
       * increased the used size by @param increment
       * @return true on success, and false otherwise, see errno.
       */
      inline bool brk(ssize_t increment) { return sbrk(increment) != (void*)-1; }

      /**
       * increased the used size by @param increment
       * @return the previous flat break pointer on success, (void*)-1 on error
       */
      void * sbrk(ssize_t increment);

      /**
       * converts an address relative to the beginning of the flat memory
       * mapping to a pointer
       */
      inline void * ptr(size_t addr) const { return addr + base_; }

      /**
       * like ptr() but return nullptr if addr is 0
       */
      inline void * ptr0(size_t addr) const { return addr > 0 ? addr + base_ : nullptr; }

      /**
       * converts a pointer to an address relative to the beginning of
       * the flat memory mapping
       */
      inline size_t addr(void * ptr) const {
        assert(ptr >= base_ && base_ + size_ <= ptr);
        return static_cast<uint8_t*> (ptr) - base_;
      }

      /**
       * helper to round up size to PAGE_SIZE
       */
      static inline size_t ceil(size_t size) {
        if (size % PAGE_SIZE)
          return size + PAGE_SIZE - size % PAGE_SIZE;
        return size;
      }

    private:
      static const long PAGE_SIZE;

      int       fd_;
      uint8_t * base_;
      size_t    size_;
      size_t    mapped_size_;
    };
  }
}

