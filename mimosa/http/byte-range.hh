#ifndef MIMOSA_HTTP_BYTE_RANGE_HH
# define MIMOSA_HTTP_BYTE_RANGE_HH

namespace mimosa
{
  namespace http
  {
    struct ByteRange
    {
      enum Type {
        kStart,
        kRange,
        kSuffix
      };

      enum Type type_;
      int64_t   start_;
      int64_t   end_;
    };
  }
}

#endif /* !MIMOSA_HTTP_BYTE_RANGE_HH */
