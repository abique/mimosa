#ifndef MIMOSA_STREAM_GZIP_STREAM_HH
# define MIMOSA_STREAM_GZIP_STREAM_HH

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class GzipStream : public Stream
    {
    public:
      GzipStream();
    };
  }
}

#endif /* !MIMOSA_STREAM_GZIP_STREAM_HH */
