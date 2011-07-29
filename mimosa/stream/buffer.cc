#include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    Buffer::Buffer(uint64_t size)
      : size_(size),
        data_(new char[size])
    {
    }

    Buffer::~Buffer()
    {
      delete data_;
      size_ = 0;
      data_ = nullptr;
    }
  }
}
