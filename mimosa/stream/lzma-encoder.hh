#ifndef MIMOSA_STREAM_LZMA_ENCODER_HH
# define MIMOSA_STREAM_LZMA_ENCODER_HH

# include "lzma.hh"

namespace mimosa
{
  namespace stream
  {
    class LzmaEncoder : public Lzma
    {
    public:
      MIMOSA_DEF_PTR(LzmaEncoder);

      LzmaEncoder(Stream::Ptr stream,
                  uint32_t    preset = 5,
                  lzma_check  check  = LZMA_CHECK_CRC64);
    };
  }
}

#endif /* !MIMOSA_STREAM_LZMA_ENCODER_HH */
