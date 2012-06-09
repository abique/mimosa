#include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    Filter::Filter(Stream::Ptr stream)
      : stream_(stream)
    {
    }

    bool
    Filter::flush()
    {
      return stream_->flush();
    }

    void
    Filter::setReadTimeout(Time timeout)
    {
      stream_->setReadTimeout(timeout);
    }

    void
    Filter::setWriteTimeout(Time timeout)
    {
      stream_->setWriteTimeout(timeout);
    }
  }
}
