#include <cerrno>

#include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    DirectFdStream::DirectFdStream(int fd, bool is_readable, bool is_writable, bool is_regular_file)
      : fd_(fd),
        is_readable_(is_readable),
        is_writable_(is_writable),
        is_regular_file_(is_regular_file)
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
      return is_regular_file_ ? ::write(fd_, data, nbytes)
        : ::melon_write(fd_, data, nbytes, timeout);
    }

    int64_t
    DirectFdStream::writev(const struct iovec *iov, int iovcnt, runtime::Time timeout)
    {
      if (!is_writable_)
      {
        errno = EINVAL;
        return -1;
      }
      return is_regular_file_ ? ::writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX)
        : ::melon_writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX, timeout);
    }

    int64_t
    DirectFdStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (!is_readable_)
      {
        errno = EINVAL;
        return -1;
      }
      return is_regular_file_ ? ::read(fd_, data, nbytes)
        : ::melon_read(fd_, data, nbytes, timeout);
    }

    int64_t
    DirectFdStream::readv(const struct iovec *iov, int iovcnt, runtime::Time timeout)
    {
      if (!is_readable_)
      {
        errno = EINVAL;
        return -1;
      }
      return is_regular_file_ ? ::readv(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX)
        : ::melon_readv(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX, timeout);
    }

    DirectFdStream::Ptr
    DirectFdStream::openFile(const char * path, int oflags, mode_t mode)
    {
      int fd = ::open(path, oflags, mode);
      if (fd < 0)
        return nullptr;
      return new DirectFdStream(fd,
                                (oflags & O_RDWR) || !(oflags & O_WRONLY),
                                (oflags & O_RDWR) || (oflags & O_WRONLY),
                                true);
    }
  }
}
