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
    Filter::flush(Time timeout)
    {
      return stream_->flush(timeout);
    }
  }
}
