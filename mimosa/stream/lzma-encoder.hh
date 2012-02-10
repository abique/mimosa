#ifndef MIMOSA_STREAM_LZMA_ENCODER_HH
# define MIMOSA_STREAM_LZMA_ENCODER_HH

# include <lzma.h>

# include "buffer.hh"
# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class LzmaEncoder : public Filter
    {
    public:
      MIMOSA_DEF_PTR(LzmaEncoder);

      LzmaEncoder(Stream::Ptr stream,
                  uint32_t    preset = 5,
                  lzma_check  check  = LZMA_CHECK_CRC64);

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);

      virtual bool flush(runtime::Time timeout = 0);

    private:
      lzma_stream lzma_;
      Buffer buffer_;
    };
  }
}

#endif /* !MIMOSA_STREAM_LZMA_ENCODER_HH */
