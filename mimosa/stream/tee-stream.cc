#include <utility>

#include <utility>

#include "tee-stream.hh"

namespace mimosa
{
  namespace stream
  {
    TeeStream::TeeStream(Stream::Ptr stream)
      : stream_(std::move(std::move(stream)))
    {
    }

    int64_t
    TeeStream::write(const char * data, uint64_t nbytes)
    {
      auto bytes = stream_->write(data, nbytes);
      if (bytes < 0)
        return bytes;
      for (auto & ostream : ostreams_)
        ostream->write(data, bytes);
      return bytes;
    }

    int64_t
    TeeStream::read(char * data, uint64_t nbytes)
    {
      auto bytes = stream_->read(data, nbytes);
      if (bytes < 0)
        return bytes;
      for (auto & istream : istreams_)
        istream->write(data, bytes);
      return bytes;
    }

    void
    TeeStream::close()
    {
      stream_->close();
      for (auto & istream : istreams_)
        istream->close();
      for (auto & ostream : ostreams_)
        ostream->close();
    }

    bool
    TeeStream::flush()
    {
      auto ret = stream_->flush();
      for (auto & istream : istreams_)
        istream->flush();
      for (auto & ostream : ostreams_)
        ostream->flush();
      return ret;
    }
  }
}
