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
    Encoder::pushInt(int64_t value, Time timeout)
    {
      char   buffer[64];
      size_t len;

      len = snprintf(buffer, sizeof (buffer), "i%lde", value);
      return output_->loopWrite(buffer, len, timeout) == (int)len;
    }

    bool
    Encoder::pushData(const char *data, size_t data_len, Time timeout)
    {
      char   buffer[64];
      size_t len;

      len = snprintf(buffer, sizeof (buffer), "%zu:", data_len);
      return output_->loopWrite(buffer, len, timeout) == (int)len &&
        output_->loopWrite(data, data_len, timeout) == (int)data_len;
    }

    bool
    Encoder::startDict(Time timeout)
    {
      return output_->loopWrite("d", 1, timeout) == 1;
    }

    bool
    Encoder::startList(Time timeout)
    {
      return output_->loopWrite("l", 1, timeout) == 1;
    }

    bool
    Encoder::end(Time timeout)
    {
      return output_->loopWrite("e", 1, timeout) == 1;
    }
  }
}
