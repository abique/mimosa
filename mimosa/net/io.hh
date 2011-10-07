#ifndef MIMOSA_NET_IO_HH
# define MIMOSA_NET_IO_HH

# include <sys/uio.h>
# include <poll.h>

# include "../runtime/time.hh"

namespace mimosa
{
  namespace net
  {
    bool waitForFdReady(int fd, int events, runtime::Time timeout);

    int64_t write(int fd, const char * data, uint64_t nbytes, runtime::Time timeout);
    int64_t writev(int fd, const ::iovec *iov, int iovcnt, runtime::Time timeout);
    int64_t read(int fd, char * data, uint64_t nbytes, runtime::Time timeout);
    int64_t readv(int fd, const ::iovec *iov, int iovcnt, runtime::Time timeout);
  }
}

#endif /* !MIMOSA_NET_IO_HH */
