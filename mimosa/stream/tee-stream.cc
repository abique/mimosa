#include "tee-stream.hh"

namespace mimosa
{
  namespace stream
  {
    TeeStream::TeeStream(Stream::Ptr stream)
      : stream_(stream)
    {
    }

    int64_t
    TeeStream::write(const char * data, uint64_t nbytes)
    {
      auto bytes = stream_->write(data, nbytes);
      if (bytes < 0)
        return bytes;
      for (auto it = ostreams_.begin(); it != ostreams_.end(); ++it)
        (*it)->write(data, bytes);
      return bytes;
    }

    int64_t
    TeeStream::read(char * data, uint64_t nbytes)
    {
      auto bytes = stream_->read(data, nbytes);
      if (bytes < 0)
        return bytes;
      for (auto it = istreams_.begin(); it != istreams_.end(); ++it)
        (*it)->write(data, bytes);
      return bytes;
    }

    void
    TeeStream::close()
    {
      stream_->close();
      for (auto it = istreams_.begin(); it != istreams_.end(); ++it)
        (*it)->close();
      for (auto it = ostreams_.begin(); it != ostreams_.end(); ++it)
        (*it)->close();
    }

    bool
    TeeStream::flush()
    {
      auto ret = stream_->flush();
      for (auto it = istreams_.begin(); it != istreams_.end(); ++it)
        (*it)->flush();
      for (auto it = ostreams_.begin(); it != ostreams_.end(); ++it)
        (*it)->flush();
      return ret;
    }
  }
}
