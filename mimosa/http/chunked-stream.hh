#pragma once

# include "../stream/filter.hh"
# include "../stream/buffered-stream.hh"

namespace mimosa
{
  namespace http
  {
    class ChunkedStream : public stream::Filter
    {
    public:
      MIMOSA_DEF_PTR(ChunkedStream);

      explicit ChunkedStream(const stream::BufferedStream::Ptr& stream);

      virtual int64_t write(const char * data, uint64_t nbytes) override;
      virtual int64_t read(char * data, uint64_t nbytes) override;

    private:
      virtual bool readChunkSize();

      stream::BufferedStream::Ptr stream_;
      uint64_t bytes_left_;
      bool first_chunk_;
      bool eof_;
    };
  }
}

