#include <memory>

#include "base16-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    Base16Encoder::Base16Encoder(Stream::Ptr stream, bool upper)
      : Filter(stream),
        base_(upper ? "0123456789ABCDEF" : "0123456789abcdef")
    {
    }

    Base16Encoder::Base16Encoder(Stream::Ptr stream, const char * base)
      : Filter(stream),
        base_(base)
    {
    }

    int64_t
    Base16Encoder::write(const char * data, uint64_t nbytes)
    {
      if (nbytes == 0)
        return 0;

      char               buffer[256];
      uint64_t           off = 0;
      const char * const end = data + nbytes;
      const char *       p   = data;

      do {
        char * b = buffer;
        char * const b_end = buffer + sizeof (buffer);

        while (p < end && b < b_end)
        {
          b[0] = base_[(*p & 0xf0) >> 4];
          b[1] = base_[*p & 0x0f];
          b += 2;
          p += 1;
        }

        int64_t wbytes = stream_->loopWrite(buffer, b - buffer);
        if (wbytes < 0) {
          if (off > 0)
            return off;
          return -1;
        }

        off += (b - buffer) >> 1;
      } while (p < end);
      return nbytes;
    }

    int64_t
    Base16Encoder::read(char * data, uint64_t nbytes)
    {
      assert(false && "not implemented");
      errno = ENOSYS;
      return -1;
    }
  }
}
