#include <zlib.h>

#include "compress.hh"
#include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    Compress::Compress(Stream::Ptr stream, int level)
      : Filter(stream),
        level_(level)
    {
    }

    int64_t
    Compress::write(const char * data, uint64_t nbytes)
    {
      Buffer buffer(::compressBound(nbytes));
      uLong dest_len = buffer.size();
      int ret = ::compress2((Bytef*)buffer.data(), &dest_len, (Bytef*)data, nbytes, level_);
      if (ret != Z_OK)
        return -1;
      return stream_->write(buffer.data(), dest_len);
    }

    int64_t
    Compress::read(char * data, uint64_t nbytes)
    {
      uLong rsize = nbytes;
      while (::compressBound(rsize) > nbytes)
      {
        if (rsize == 0)
          return 0;
        rsize = rsize >> 1;
      }

      Buffer buffer(rsize);
      rsize = stream_->read(buffer.data(), rsize);
      if (rsize == (uLong)-1)
        return -1;
      int ret = ::compress2((Bytef*)data, &nbytes, (Bytef*)buffer.data(), rsize, level_);
      if (ret != Z_OK)
        return -1; // kind of fatal error at this point if we don't save
                   // our input buffer.
      return nbytes;
    }

    bool
    Compress::flush()
    {
      return stream_->flush();
    }

    void
    Compress::close()
    {
      flush();
      stream_->close();
    }
  }
}
