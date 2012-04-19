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
    TeeStream::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      auto bytes = stream_->write(data, nbytes, timeout);
      if (bytes < 0)
        return bytes;
      for (auto it = ostreams_.begin(); it != ostreams_.end(); ++it)
        (*it)->write(data, bytes, 0);
      return bytes;
    }

    int64_t
    TeeStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      auto bytes = stream_->read(data, nbytes, timeout);
      if (bytes < 0)
        return bytes;
      for (auto it = istreams_.begin(); it != istreams_.end(); ++it)
        (*it)->write(data, bytes, 0);
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
    TeeStream::flush(runtime::Time timeout)
    {
      auto ret = stream_->flush(timeout);
      for (auto it = istreams_.begin(); it != istreams_.end(); ++it)
        (*it)->flush(0);
      for (auto it = ostreams_.begin(); it != ostreams_.end(); ++it)
        (*it)->flush(0);
      return ret;
    }
  }
}
