#ifndef MIMOSA_STREAM_PERCENT_ENCODER_HH
# define MIMOSA_STREAM_PERCENT_ENCODER_HH

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

      PercentEncoder(Stream::Ptr stream, uri::EncodingType enc = uri::kRfc3986);

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);

    protected:
      uri::EncodingType enc_;
    };
  }
}

#endif /* !MIMOSA_STREAM_PERCENT_ENCODER_HH */
