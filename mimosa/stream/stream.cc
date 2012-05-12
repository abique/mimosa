#include <cerrno>

#include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    int64_t
    Stream::writev(const struct iovec * iov, int iovcnt, Time timeout)
    {
      int64_t bytes = 0;
      for (int i = 0; i < iovcnt; ++i)
      {
        int64_t ret = loopWrite((const char *)iov[i].iov_base, iov[i].iov_len, timeout);
        if (ret <= 0)
        {
          if (bytes > 0)
            return bytes;
          return ret;
        }

        bytes += ret;

        if (ret != iov[i].iov_len)
          return bytes;
      }
      return bytes;
    }

    int64_t
    Stream::readv(const struct iovec * iov, int iovcnt, Time timeout)
    {
      int64_t bytes = 0;
      for (int i = 0; i < iovcnt; ++i)
      {
        int64_t ret = loopRead((char *)iov[i].iov_base, iov[i].iov_len, timeout);
        if (ret <= 0)
        {
          if (bytes > 0)
            return bytes;
          return ret;
        }

        bytes += ret;

        if (ret != iov[i].iov_len)
          return bytes;
      }
      return bytes;
    }

    int64_t
    Stream::loopRead(char * data, const uint64_t nbytes, Time timeout)
    {
      for (int64_t bytes_left = nbytes; bytes_left > 0; )
      {
        auto bytes = read(data, bytes_left, timeout);
        if (bytes <= 0)
        {
          if (errno == EAGAIN)
            continue;
          return nbytes - bytes_left;
        }
        data       += bytes;
        bytes_left -= bytes;
      }
      return nbytes;
    }

    int64_t
    Stream::loopWrite(const char * data, const uint64_t nbytes, Time timeout)
    {
      for (int64_t bytes_left = nbytes; bytes_left > 0; )
      {
        auto bytes = write(data, bytes_left, timeout);
        if (bytes <= 0)
        {
          if (errno == EAGAIN)
            continue;
          return nbytes - bytes_left;
        }
        data       += bytes;
        bytes_left -= bytes;
      }
      return nbytes;
    }
  }
}
