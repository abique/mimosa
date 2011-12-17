#include <cstring>
#include <memory>

#include "base16-decoder.hh"

namespace mimosa
{
  namespace stream
  {
    Base16Decoder::Base16Decoder(Stream::Ptr stream, const char * base)
      : Filter(stream),
        base_(base)
    {
    }

    int
    Base16Decoder::decodeByte(char c) const
    {
      if (base_)
        return ::strchr(base_, c) - base_;

      if ('0' <= c && c <= '9')
        return c - '0';
      if ('a' <= c && c <= 'f')
        return c - 'a';
      if ('A' <= c && c <= 'F')
        return c - 'A';
      return 0;
    }

    int64_t
    Base16Decoder::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (nbytes < 2)
        return 0;

      std::unique_ptr<char> buffer(new char[nbytes / 2]);

      char * b = buffer.get();
      const char * const end = data + (nbytes & ~1);
      const char * p = data;

      while (p < end)
      {
        b[0] = (decodeByte(p[0]) << 4) | decodeByte(p[1]);
        b += 1;
        p += 2;
      }

      return stream_->loopWrite(buffer.get(), 2 * nbytes, timeout);
    }

    int64_t
    Base16Decoder::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false && "not implemented");
      return -1;
    }
  }
}
