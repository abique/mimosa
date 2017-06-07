#pragma once

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    enum Coding
    {
      kCodingIdentity     = 1 << 0,
      kCodingDeflate      = 1 << 1,
      kCodingGzip         = 1 << 2,
      kCodingExi          = 1 << 3,
      kCodingCompress     = 1 << 4,
      kCodingPack200_gzip = 1 << 5,
      kCodingChunked      = 1 << 6,
      kCodingSdch         = 1 << 7,
    };
  }
}

