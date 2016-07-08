#pragma once

#include "../compat/uio.hh"

#include "../time.hh"

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

