#include <poll.h>

#include "net-fd-stream.hh"
#include "../net/io.hh"

namespace mimosa
{
  namespace stream
  {
    NetFdStream::NetFdStream(int fd, bool own_fd)
      : DirectFdStream(fd, own_fd)
    {
    }

    int64_t
    NetFdStream::write(const char * data, uint64_t nbytes, Time timeout)
    {
      return net::write(fd(), data, nbytes, timeout);
    }

    int64_t
    NetFdStream::writev(const struct iovec *iov, int iovcnt, Time timeout)
    {
      return net::writev(fd(), iov, iovcnt, timeout);
    }

    int64_t
    NetFdStream::read(char * data, uint64_t nbytes, Time timeout)
    {
      return net::read(fd(), data, nbytes, timeout);
    }

    int64_t
    NetFdStream::readv(const struct iovec *iov, int iovcnt, Time timeout)
    {
      return net::readv(fd(), iov, iovcnt, timeout);
    }
  }
}
