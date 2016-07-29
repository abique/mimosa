#include "../net/common.hh"

#include "net-fd-stream.hh"
#include "../net/io.hh"

namespace mimosa
{
  namespace stream
  {
    NetFdStream::NetFdStream(int fd, bool own_fd)
      : DirectFdStream(fd, own_fd),
        read_timeout_(0),
        write_timeout_(0),
        read_idle_timeout_(0),
        write_idle_timeout_(0)
    {
    }

    int64_t
    NetFdStream::write(const char * data, uint64_t nbytes)
    {
      return net::write(fd_, data, nbytes, write_timeout_);
    }

    int64_t
    NetFdStream::writev(const struct iovec *iov, int iovcnt)
    {
      return net::writev(fd_, iov, iovcnt, write_timeout_);
    }

    int64_t
    NetFdStream::read(char * data, uint64_t nbytes)
    {
      return net::read(fd_, data, nbytes, read_timeout_);
    }

    int64_t
    NetFdStream::readv(const struct iovec *iov, int iovcnt)
    {
      return net::readv(fd_, iov, iovcnt, read_timeout_);
    }

    void
    NetFdStream::setReadTimeout(Time timeout)
    {
      read_timeout_ = timeout;
    }

    void
    NetFdStream::setWriteTimeout(Time timeout)
    {
      write_timeout_ = timeout;
    }

    void
    NetFdStream::setReadIdleTimeout(Time timeout)
    {
      if (timeout == read_idle_timeout_)
        return;

      struct ::timeval tv = toTimeVal(timeout);
      read_idle_timeout_ = timeout;
      ::setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv));
    }

    void
    NetFdStream::setWriteIdleTimeout(Time timeout)
    {
      if (timeout == write_idle_timeout_)
        return;

      struct ::timeval tv = toTimeVal(timeout);
      write_idle_timeout_ = timeout;
      ::setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof (tv));
    }
  }
}
