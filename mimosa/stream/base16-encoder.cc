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
    Base16Encoder::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (nbytes == 0)
        return 0;

      std::unique_ptr<char> buffer(new char[2 * nbytes]);

      char * b = buffer.get();
      const char * const end = data + nbytes;
      const char * p = data;

      while (p < end)
      {
        b[0] = base_[(*p & 0xf0) >> 4];
        b[1] = base_[*p & 0x0f];
        b += 2;
        p += 1;
      }

      return stream_->loopWrite(buffer.get(), 2 * nbytes, timeout);
    }

    int64_t
    Base16Encoder::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false && "not implemented");
      return -1;
    }
  }
}
