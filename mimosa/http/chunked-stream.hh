#ifndef MIMOSA_HTTP_CHUNKED_STREAM_HH
# define MIMOSA_HTTP_CHUNKED_STREAM_HH

# include "../stream/filter.hh"

namespace mimosa
{
  namespace http
  {
    class ChunkedStream : public stream::Filter
    {
    public:
      MIMOSA_DEF_PTR(ChunkedStream);

      ChunkedStream(stream::Stream::Ptr stream);

      virtual int64_t write(const char * data, uint64_t nbytes) override;
      virtual int64_t read(char * data, uint64_t nbytes) override;
    };
  }
}

#endif /* !MIMOSA_HTTP_CHUNKED_STREAM_HH */
