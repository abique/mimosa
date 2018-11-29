#include <cerrno>
#include <utility>

#include "percent-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    PercentEncoder::PercentEncoder(Stream::Ptr       stream,
                                   uri::EncodingType enc)
      : Filter(std::move(stream)),
        enc_(enc)
    {
    }

    int64_t
    PercentEncoder::write(const char * data, uint64_t nbytes)
    {
      std::string out;

      uri::percentEncode(data, nbytes, &out, enc_);
      if (stream_->loopWrite(out.data(), out.size()) == static_cast<int64_t> (out.size()))
        return nbytes;
      return -1;
    }

    int64_t
    PercentEncoder::read(char * /*data*/, uint64_t /*nbytes*/)
    {
      assert(false && "NOT IMPLEMENTED");
      errno = ENOSYS;
      return -1;
    }
  }
}
