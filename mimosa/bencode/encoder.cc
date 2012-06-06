#include "encoder.hh"

namespace mimosa
{
  namespace bencode
  {
    Encoder::Encoder(stream::Stream::Ptr output)
      : output_(output)
    {
    }

    bool
    Encoder::pushInt(int64_t value)
    {
      char   buffer[64];
      size_t len;

      len = snprintf(buffer, sizeof (buffer), "i%lde", value);
      return output_->loopWrite(buffer, len) == (int)len;
    }

    bool
    Encoder::pushData(const char *data, size_t data_len)
    {
      char   buffer[64];
      size_t len;

      len = snprintf(buffer, sizeof (buffer), "%zu:", data_len);
      return output_->loopWrite(buffer, len) == (int)len &&
        output_->loopWrite(data, data_len) == (int)data_len;
    }

    bool
    Encoder::startDict()
    {
      return output_->loopWrite("d", 1) == 1;
    }

    bool
    Encoder::startList()
    {
      return output_->loopWrite("l", 1) == 1;
    }

    bool
    Encoder::end()
    {
      return output_->loopWrite("e", 1) == 1;
    }
  }
}
