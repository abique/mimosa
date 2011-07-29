#include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    DirectFdStream::DirectFdStream(int fd, bool readable, bool writable)
      : fd_(fd)
    {
    }

    DirectFdStream::~DirectFdStream()
    {
      ::close(fd_);
      fd_ = -1;
    }

    int64_t
    DirectFdStream::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (!is_writable_)
      {
        errno = EINVAL;
        return -1;
      }
      return ::mimosa_write(fd_, data, nbytes, timeout);
    }

    int64_t writev(const struct iovec *iov, int iovcnt, runtime::Time timeout)
    {
      if (!is_writable_)
      {
        errno = EINVAL;
        return -1;
      }
      return ::mimosa_writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX, timeout);
    }

    int64_t
    DirectFdStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (!is_readable_)
      {
        errno = EINVAL;
        return -1;
      }
      return ::mimosa_read(fd_, data, nbytes, timeout);
    }
  }
}
