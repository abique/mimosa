#include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    int64_t
    Stream::writev(const struct iovec * iov, int iovcnt)
    {
      int64_t wbytes = 0;
      for (int i = 0; i < iovcnt; ++i)
      {
        int64_t ret = write(iov[i].iov_base, iov[i].iov_len, timeout);
        if (ret <= 0)
        {
          if (wbytes > 0)
            return wbytes;
          return ret;
        }
      }
      return wbytes;
    }
  }
}
