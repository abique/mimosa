#include <cstring>

#include "base64-decoder.hh"

namespace mimosa
{
  namespace stream
  {
    static const char BASE16_DICT[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/";

    Base64Decoder::Base64Decoder(Stream::Ptr stream, const char * base)
      : Filter(stream),
        base_(base ? : BASE16_DICT),
        off_(0),
        pad_(0)
    {
    }

    uint8_t
    Base64Decoder::decodeByte(uint8_t c) const
    {
      if (base_)
        return ::strchr(base_, c) - base_;

      if ('0' <= c && c <= '9')
        return c - '0';
      if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
      if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
      return 0;
    }

    int64_t
    Base64Decoder::write(const char * data, uint64_t nbytes)
    {
      const char *in = data;
      const char * const end = in + nbytes;

      for (; in < end; ++in) {
        const char *found = strchr(base_, *in);

        if (*in == '=') {
          found = base_;
          ++pad_;
        }

        // discard not found
        if (!found)
          continue;

        buff_[off_] = found - base_;
        ++off_;

        if (off_ != 4)
          continue;

        char decode[3];
        decode[0] = (buff_[0] << 2) | (buff_[1] >> 4);
        decode[1] = (buff_[1] << 4) | (buff_[2] >> 2);
        decode[2] = (buff_[2] << 6) | buff_[3];

        stream_->loopWrite(decode, 3 - pad_);

        off_ = 0;
        pad_ = 0;
      }
      return nbytes;
    }

    int64_t
    Base64Decoder::read(char * /*data*/, uint64_t /*nbytes*/)
    {
      assert(false && "not implemented");
      errno = ENOSYS;
      return -1;
    }
  }
}
