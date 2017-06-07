#pragma once

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
