#include <cstdlib>
#include <cstring>
#include <exception>

#include "zlib-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    ZlibEncoder::ZlibEncoder(Stream::Ptr stream,
                             int         level,
                             int         method,
                             int         window_bits,
                             int         mem_level,
                             int         strategy)
      : Filter(stream),
        buffer_(4096)
    {
      ::memset(&zstream_, 0, sizeof (zstream_));

      if (::deflateInit2(&zstream_, level, method, window_bits, mem_level, strategy)
          != Z_OK)
        throw std::exception();
    }

    ZlibEncoder::~ZlibEncoder()
    {
      ::deflateEnd(&zstream_);
    }

    int64_t
    ZlibEncoder::write(const char * data, uint64_t nbytes)
    {
      zstream_.next_in = (Bytef*)data;
      zstream_.avail_in = nbytes;

      do {
        zstream_.next_out = (Bytef*)buffer_.data();
        zstream_.avail_out = buffer_.size();

        int ret = ::deflate(&zstream_, Z_NO_FLUSH);

        if (ret != Z_OK && ret != Z_STREAM_END)
          return -1;

        if (buffer_.size() > zstream_.avail_out &&
            stream_->loopWrite(buffer_.data(), buffer_.size() - zstream_.avail_out)
            != static_cast<int64_t> (buffer_.size() - zstream_.avail_out))
          return -1;

        if (ret == Z_STREAM_END)
          return nbytes - zstream_.avail_in;

      } while (zstream_.avail_in > 0);

      return nbytes;
    }

    int64_t
    ZlibEncoder::read(char * /*data*/, uint64_t /*nbytes*/)
    {
      assert(false);
      return -1;
    }

    bool
    ZlibEncoder::flush()
    {
      zstream_.next_in = NULL;
      zstream_.avail_in = 0;

      while (true)
      {
        zstream_.next_out = (uint8_t*)buffer_.data();
        zstream_.avail_out = buffer_.size();
        int ret = ::deflate(&zstream_, Z_FINISH);

        if (buffer_.size() > zstream_.avail_out &&
            stream_->loopWrite(buffer_.data(), buffer_.size() - zstream_.avail_out) < 0)
          return false;

        if (ret == Z_STREAM_END)
          return stream_->flush();

        if (ret != Z_OK)
          return false;
      }
    }
  }
}
