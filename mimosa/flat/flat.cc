#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "flat.hh"

namespace mimosa
{
  namespace flat
  {
    static const long  PAGE_SIZE = sysconf(_SC_PAGE_SIZE);

    Flat::Flat(const std::string & path, int oflags, mode_t mode)
      : fd_(-1),
        base_(nullptr),
        size_(0),
        mapped_size_(PAGE_SIZE)
    {
      int prot  = 0;
      int flags = MAP_SHARED;

      if (oflags & O_WRONLY)
        prot = PROT_WRITE;
      else if (oflags & O_RDWR)
        prot = PROT_WRITE | PROT_READ;
      else
        prot = PROT_READ;

      if (!path.empty())
      {
        struct ::stat st;

        if (::stat(path.c_str(), &st))
          return;

        size_        = st.st_size;
        mapped_size_ = ceil(st.st_size);

        fd_ = ::open(path.c_str(), oflags, mode);
        if (fd_ < 0)
          return;
      }
      else
        flags |= MAP_ANONYMOUS;

      base_ = (uint8_t*)::mmap(nullptr, mapped_size_, prot, flags, fd_, 0);
      if (base_ == MAP_FAILED)
        base_ = nullptr;
    }

    Flat::~Flat()
    {
      if (base_)
        ::munmap(base_, mapped_size_);

      if (fd_)
      {
        if (size_ < mapped_size_)
          ::ftruncate(fd_, size_);
        ::close(fd_);
      }

      fd_          = -1;
      base_        = nullptr;
      size_        = 0;
      mapped_size_ = 0;
    }

    void *
    Flat::sbrk(ssize_t increment)
    {
      size_t old_size = size_;
      size_t new_size = increment + size_;

      if (increment < 0)
        assert(static_cast<ssize_t> (new_size) >= 0);

      if (new_size > mapped_size_ &&
          !reserve(new_size))
        return (void*)-1;

      new_size = size_;
      return base_ + old_size;
    }

    bool
    Flat::reserve(size_t size)
    {
      size = ceil(size);

      if (size <= mapped_size_)
        return true;

      void *addr = mremap(base_, mapped_size_, size, MREMAP_MAYMOVE);
      if (addr == MAP_FAILED)
        return false;

      base_        = (uint8_t*)addr;
      mapped_size_ = size;
      return true;
    }
  }
}
