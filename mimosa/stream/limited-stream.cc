#include <cerrno>

#include "limited-stream.hh"

namespace mimosa
{
  namespace stream
  {
    LimitedStream::LimitedStream(Stream::Ptr stream)
      : stream_(stream),
        rbytes_left_(0),
        wbytes_left_(0)
    {
    }

    int64_t
    LimitedStream::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      uint64_t can_write = nbytes > wbytes_left_ ? wbytes_left_ : nbytes;
      if (can_write == 0)
        return 0;

      int64_t wbytes = stream_->write(data, can_write, timeout);
      if (wbytes <= 0)
        return wbytes;

      wbytes_left_ -= wbytes;
      return wbytes;
    }

    int64_t
    LimitedStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      uint64_t can_read = nbytes > rbytes_left_ ? rbytes_left_ : nbytes;
      if (can_read == 0)
        return 0;

      int64_t rbytes = stream_->write(data, can_read, timeout);
      if (rbytes <= 0)
        return rbytes;

      rbytes_left_ -= rbytes;
      return rbytes;
    }
  }
}
