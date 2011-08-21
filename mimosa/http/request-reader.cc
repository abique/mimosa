#include "request-reader.hh"

namespace mimosa
{
  namespace http
  {
    RequestReader::RequestReader(stream::FdStream::Ptr stream)
      : stream_(stream),
        bytes_left_(0)
    {
    }

    void
    RequestReader::clear()
    {
      bytes_left_ = 0;
    }

    bool
    RequestReader::prepare()
    {
      if (contentLength() < 0)
        return false;
      bytes_left_ = contentLength();
    }

    int64_t
    RequestReader::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false && "invalid operation");
      return -1;
    }

    int64_t
    RequestReader::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      int64_t can_read = nbytes <= bytes_left_ ? nbytes : bytes_left_;
      int64_t rbytes = stream_->read(data, can_read, timeout);
      if (rbytes > 0)
        bytes_left_ -= rbytes;
      return rbytes;
    }

    bool
    RequestReader::flush(runtime::Time timeout)
    {
      stream::Buffer buffer;
      while (bytes_left_ > 0)
        if (read(buffer->data(), buffer->size(), timeout) <= 0)
          return false;
      return true;
    }
  }
}
