#pragma once

# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class Base16Decoder : public Filter
    {
    public:
      MIMOSA_DEF_PTR(Base16Decoder);

      Base16Decoder(Stream::Ptr stream, const char * base = NULL);

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      uint8_t decodeByte(uint8_t c) const;

    private:
      const char * base_;
    };
  }
}

