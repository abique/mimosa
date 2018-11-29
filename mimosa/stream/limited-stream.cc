#include <cerrno>
#include <algorithm>
#include <utility>

#include "limited-stream.hh"

namespace mimosa
{
  namespace stream
  {
    LimitedStream::LimitedStream(Stream::Ptr stream)
      : stream_(std::move(stream)),
        rbytes_left_(0),
        wbytes_left_(0)
    {
    }

    int64_t
    LimitedStream::write(const char * data, uint64_t nbytes)
    {
      uint64_t can_write = std::min(nbytes, wbytes_left_);
      if (can_write == 0)
        return 0;

      int64_t wbytes = stream_->write(data, can_write);
      if (wbytes > 0)
        wbytes_left_ -= wbytes;
      return wbytes;
    }

    int64_t
    LimitedStream::read(char * data, uint64_t nbytes)
    {
      uint64_t can_read = std::min(nbytes, rbytes_left_);
      if (can_read == 0)
        return 0;

      int64_t rbytes = stream_->read(data, can_read);
      if (rbytes > 0)
        rbytes_left_ -= rbytes;
      return rbytes;
    }
  }
}
