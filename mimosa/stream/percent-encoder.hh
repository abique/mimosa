#pragma once

# include "filter.hh"
# include "../uri/percent-encoding.hh"

namespace mimosa
{
  namespace stream
  {
    class PercentEncoder : public Filter
    {
    public:
      MIMOSA_DEF_PTR(PercentEncoder);

      explicit PercentEncoder(Stream::Ptr stream, uri::EncodingType enc = uri::kRfc3986);

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

    protected:
      uri::EncodingType enc_;
    };
  }
}

