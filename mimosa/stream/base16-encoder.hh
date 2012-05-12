#ifndef MIMOSA_STREAM_BASE16_ENCODER_HH
# define MIMOSA_STREAM_BASE16_ENCODER_HH

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

      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0);

    private:
      const char * base_;
    };
  }
}

#endif /* !MIMOSA_STREAM_BASE16_ENCODER_HH */
