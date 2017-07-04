#pragma once

# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class Base16Encoder : public Filter
    {
    public:
      MIMOSA_DEF_PTR(Base16Encoder);

      Base16Encoder(Stream::Ptr stream, bool upper = false);
      Base16Encoder(Stream::Ptr stream, const char * base);

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      static int64_t write(Stream::Ptr stream, const char *data, uint64_t nbytes, bool upper = false);

    private:
      const char * base_;
    };
  }
}

