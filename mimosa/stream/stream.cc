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

        if (ret != static_cast<int64_t> (iov[i].iov_len))
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

        if (ret != static_cast<int64_t> (iov[i].iov_len))
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
      int64_t bytes;

      for (int64_t bytes_left = nbytes; bytes_left > 0; )
      {
        bytes = write(data, bytes_left, timeout);
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
    Stream::loopWritev(struct iovec *iov, int iovcnt, Time timeout)
    {
      uint64_t total = 0;
      ssize_t  nbytes;

      while (iovcnt > 0)
      {
        nbytes = writev(iov, iovcnt, timeout);
        if (nbytes < 0)
        {
          if (errno == EAGAIN)
            continue;
          return total;
        }
        total += nbytes;

        while (iovcnt > 0 && nbytes > 0)
        {
          if (nbytes > static_cast<ssize_t> (iov->iov_len))
          {
            nbytes -= iov->iov_len;
            --iovcnt;
            ++iov;
          }
          else
          {
            iov->iov_len -= nbytes;
            iov->iov_base = static_cast<uint8_t*> (iov->iov_base) + nbytes;
            nbytes = 0;
          }
        }
      }

      return total;
    }
  }
}
