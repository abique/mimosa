#ifndef MIMOSA_STREAM_FILTER_HH
# define MIMOSA_STREAM_FILTER_HH

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class Filter : public Stream
    {
    public:
      MIMOSA_DEF_PTR(Filter);

      Filter(Stream::Ptr stream);

      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0) = 0;
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0) = 0;

      virtual bool flush(Time timeout = 0);

    protected:
      Stream::Ptr stream_;
    };
  }
}

#endif /* !MIMOSA_STREAM_FILTER_HH */
