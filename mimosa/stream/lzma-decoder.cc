#include <utility>

#include "lzma-decoder.hh"

namespace mimosa
{
  namespace stream
  {
    LzmaDecoder::LzmaDecoder(Stream::Ptr stream, uint64_t memlimit, uint32_t flags)
      : Lzma(std::move(stream))
    {
      if (lzma_auto_decoder(&lzma_, memlimit, flags) != LZMA_OK)
        assert(false);
    }
  }
}
