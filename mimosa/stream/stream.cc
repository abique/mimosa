#include <cerrno>

#include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    int64_t
    Stream::writev(const struct iovec * iov, int iovcnt, runtime::Time timeout)
    {
      int64_t bytes = 0;
      for (int i = 0; i < iovcnt; ++i)
      {
        int64_t ret = write((const char *)iov[i].iov_base, iov[i].iov_len, timeout);
        if (ret <= 0)
        {
          if (bytes > 0)
            return bytes;
          return ret;
        }
        bytes += ret;
      }
      return bytes;
    }

    int64_t
    Stream::readv(const struct iovec * iov, int iovcnt, runtime::Time timeout)
    {
      int64_t bytes = 0;
      for (int i = 0; i < iovcnt; ++i)
      {
        int64_t ret = read((char *)iov[i].iov_base, iov[i].iov_len, timeout);
        if (ret <= 0)
        {
          if (bytes > 0)
            return bytes;
          return ret;
        }
        bytes += ret;
      }
      return bytes;
    }

    int64_t
    Stream::loopRead(char * data, const uint64_t nbytes, runtime::Time timeout)
    {
      uint64_t bytes_left = nbytes;
      do {
        auto bytes = read(data, bytes_left, timeout);
        if (bytes <= 0)
        {
          if (errno == EAGAIN)
            continue;
          return nbytes - bytes_left;
        }
        data       += bytes;
        bytes_left -= bytes;
      } while (bytes_left > 0);
      return nbytes;
    }

    int64_t
    Stream::loopWrite(const char * data, const uint64_t nbytes, runtime::Time timeout)
    {
      uint64_t bytes_left = nbytes;
      do {
        auto bytes = write(data, bytes_left, timeout);
        if (bytes <= 0)
        {
          if (errno == EAGAIN)
            continue;
          return nbytes - bytes_left;
        }
        data       += bytes;
        bytes_left -= bytes;
      } while (bytes_left > 0);
      return nbytes;
    }
  }
}
