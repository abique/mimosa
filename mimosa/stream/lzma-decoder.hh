#pragma once

# include "lzma.hh"

namespace mimosa
{
  namespace stream
  {
    class LzmaDecoder : public Lzma
    {
    public:
      MIMOSA_DEF_PTR(LzmaDecoder);

      explicit LzmaDecoder(Stream::Ptr stream,
                           uint64_t    memlimit = UINT64_MAX,
                           uint32_t    flags = 0);
    };
  }
}

