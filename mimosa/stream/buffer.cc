#include <cstdlib>

#include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    Buffer::Buffer(uint64_t size)
      : size_(size),
        data_(reinterpret_cast<char *>(::malloc(size + 4)))
    {
      data_[size_ - 3] = 0;
      data_[size_ - 2] = 0;
      data_[size_ - 1] = 0;
      data_[size_]     = 0;
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
      data_            = reinterpret_cast<char *>(::realloc(data_, size + 4));
      size_            = size;
      data_[size_ - 3] = 0;
      data_[size_ - 2] = 0;
      data_[size_ - 1] = 0;
      data_[size_]     = 0;
    }
  }
}
