#include <zlib.h>

#include "compress-stream.hh"
#include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    CompressStream::CompressStream(Stream::Ptr stream, int level)
      : stream_(stream),
        level_(level)
    {
    }

    int64_t
    CompressStream::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      Buffer buffer(::compressBound(nbytes));
      uLong dest_len = buffer.size();
      int ret = ::compress2((Bytef*)buffer.data(), &dest_len, (Bytef*)data, nbytes, level_);
      if (ret != Z_OK)
        return -1;
      return stream_->write(buffer.data(), dest_len, timeout);
    }

    int64_t
    CompressStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      uLong rsize = nbytes;
      while (::compressBound(rsize) > nbytes)
      {
        if (rsize == 0)
          return 0;
        --rsize;
      }

      Buffer buffer(rsize);
      rsize = stream_->read(buffer.data(), rsize, timeout);
      if (rsize == -1)
        return -1;
      uLong nbytes_ = nbytes;
      int ret = ::compress2((Bytef*)data, &nbytes, (Bytef*)buffer.data(), rsize, level_);
      if (ret != Z_OK)
        return -1; // kind of fatal error at this point if we don't save
                   // our input buffer.
      return nbytes;
    }
  }
}
