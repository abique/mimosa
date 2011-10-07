#include <cerrno>

#include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    DirectFdStream::DirectFdStream(int fd, bool own_fd)
      : fd_(fd),
        own_fd_(own_fd)
    {
    }

    DirectFdStream::~DirectFdStream()
    {
      if (own_fd_ && fd_ >= 0)
      {
        ::close(fd_);
        fd_ = -1;
      }
    }

    int64_t
    DirectFdStream::write(const char * data, uint64_t nbytes, runtime::Time /*timeout*/)
    {
      return ::write(fd_, data, nbytes);
    }

    int64_t
    DirectFdStream::writev(const struct iovec *iov, int iovcnt, runtime::Time /*timeout*/)
    {
      return ::writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX);
    }

    int64_t
    DirectFdStream::read(char * data, uint64_t nbytes, runtime::Time /*timeout*/)
    {
      return ::read(fd_, data, nbytes);
    }

    int64_t
    DirectFdStream::readv(const struct iovec *iov, int iovcnt, runtime::Time /*timeout*/)
    {
      return ::readv(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX);
    }

    void
    DirectFdStream::close()
    {
      assert(own_fd_);
      if (own_fd_ && fd_ >= 0)
      {
        int fd = fd_;
        fd_    = -1;
        ::close(fd);
      }
    }
  }
}
