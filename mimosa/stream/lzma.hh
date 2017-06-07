#pragma once

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

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      virtual bool flush();

    protected:
      Lzma(Stream::Ptr stream);

      lzma_stream lzma_;
      Buffer buffer_;
    };
  }
}

