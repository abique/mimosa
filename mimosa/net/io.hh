#ifndef MIMOSA_NET_IO_HH
# define MIMOSA_NET_IO_HH

# include <sys/uio.h>
# include <poll.h>

# include "../time.hh"

namespace mimosa
{
  namespace net
  {
    bool waitForFdReady(int fd, int events, Time timeout);

    int64_t write(int fd, const char * data, uint64_t nbytes, Time timeout);
    int64_t writev(int fd, const ::iovec *iov, int iovcnt, Time timeout);
    int64_t read(int fd, char * data, uint64_t nbytes, Time timeout);
    int64_t readv(int fd, const ::iovec *iov, int iovcnt, Time timeout);
  }
}

#endif /* !MIMOSA_NET_IO_HH */
