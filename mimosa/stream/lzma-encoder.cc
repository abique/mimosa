#include <utility>

#include "lzma-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    LzmaEncoder::LzmaEncoder(Stream::Ptr stream, uint32_t preset, lzma_check check)
      : Lzma(std::move(stream))
    {
      if (lzma_easy_encoder(&lzma_, preset, check) != LZMA_OK)
        assert(false);
    }
  }
}
