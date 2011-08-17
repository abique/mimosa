#include <cerrno>

#include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    DirectFdStream::DirectFdStream(int fd, bool is_readable, bool is_writable)
      : fd_(fd),
        is_readable_(is_readable),
        is_writable_(is_writable)
    {
    }

    DirectFdStream::~DirectFdStream()
    {
      ::melon_close(fd_);
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
      return ::melon_write(fd_, data, nbytes, timeout);
    }

    int64_t
    DirectFdStream::writev(const struct iovec *iov, int iovcnt, runtime::Time timeout)
    {
      if (!is_writable_)
      {
        errno = EINVAL;
        return -1;
      }
      return ::melon_writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX, timeout);
    }

    int64_t
    DirectFdStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (!is_readable_)
      {
        errno = EINVAL;
        return -1;
      }
      return ::melon_read(fd_, data, nbytes, timeout);
    }

    int64_t
    DirectFdStream::readv(const struct iovec *iov, int iovcnt, runtime::Time timeout)
    {
      if (!is_readable_)
      {
        errno = EINVAL;
        return -1;
      }
      return ::melon_readv(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX, timeout);
    }
  }
}
