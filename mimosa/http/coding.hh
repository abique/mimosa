#ifndef MIMOSA_HTTP_CODING_HH
# define MIMOSA_HTTP_CODING_HH

namespace mimosa
{
  namespace http
  {
    enum TransferEncoding
    {
      kCodingIdentity     = 0,
      kCodingDeflate      = 1 << 0,
      kCodingGzip         = 1 << 1,
      kCodingExi          = 1 << 2,
      kCodingCompress     = 1 << 3,
      kCodingPack200_gzip = 1 << 4,
      kCodingChunked      = 1 << 5,
    }
  }
}

#endif /* !MIMOSA_HTTP_CODING_HH */
