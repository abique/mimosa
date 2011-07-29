#include <cstdlib>

#include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    Buffer::Buffer(uint64_t size)
      : size_(size),
        data_(::malloc(size))
    {
    }

    Buffer::~Buffer()
    {
      free(data_);
      size_ = 0;
      data_ = nullptr;
    }

    void
    Buffer::resize(uint64_t size)
    {
      data_ = ::realloc(data_, size);
      size_ = size;
    }
  }
}
