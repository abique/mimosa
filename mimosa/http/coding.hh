#ifndef MIMOSA_HTTP_CODING_HH
# define MIMOSA_HTTP_CODING_HH

namespace mimosa
{
  namespace http
  {
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

#endif /* !MIMOSA_HTTP_CODING_HH */
