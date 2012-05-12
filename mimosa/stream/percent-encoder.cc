#include "percent-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    PercentEncoder::PercentEncoder(Stream::Ptr       stream,
                                   uri::EncodingType enc)
      : Filter(stream),
        enc_(enc)
    {
    }

    int64_t
    PercentEncoder::write(const char * data, uint64_t nbytes, Time timeout)
    {
      std::string out;

      uri::percentEncode(data, nbytes, &out, enc_);
      if (stream_->loopWrite(out.data(), out.size(), timeout) == out.size())
        return nbytes;
      return -1;
    }

    int64_t
    PercentEncoder::read(char * data, uint64_t nbytes, Time timeout)
    {
      assert(false && "NOT IMPLEMENTED");
      errno = ENOSYS;
      return -1;
    }
  }
}
