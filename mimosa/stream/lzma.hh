#ifndef MIMOSA_STREAM_LZMA_HH
# define MIMOSA_STREAM_LZMA_HH

# include <lzma.h>

# include "buffer.hh"
# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class Lzma : public Filter
    {
    public:
      MIMOSA_DEF_PTR(Lzma);

      virtual ~Lzma();

      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0);

      virtual bool flush(Time timeout = 0);

    protected:
      Lzma(Stream::Ptr stream);

      lzma_stream lzma_;
      Buffer buffer_;
    };
  }
}

#endif /* !MIMOSA_STREAM_LZMA_HH */
