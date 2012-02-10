#ifndef MIMOSA_STREAM_LZMA_DECODER_HH
# define MIMOSA_STREAM_LZMA_DECODER_HH

# include "lzma.hh"

namespace mimosa
{
  namespace stream
  {
    class LzmaDecoder : public Lzma
    {
    public:
      MIMOSA_DEF_PTR(LzmaDecoder);

      LzmaDecoder(Stream::Ptr stream,
                  uint64_t    memlimit = UINT64_MAX,
                  uint32_t    flags = 0);
    };
  }
}

#endif /* !MIMOSA_STREAM_LZMA_DECODER_HH */
