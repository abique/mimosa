#pragma once

# include <zlib.h>

# include "buffer.hh"
# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class ZlibEncoder : public Filter
    {
    public:
      MIMOSA_DEF_PTR(ZlibEncoder);

      /**
       * @see zlib.h, deflateInit2 to get the parameters definition.
       */
      ZlibEncoder(Stream::Ptr stream,
                  int         level,
                  int         method,
                  int         window_bits,
                  int         mem_level,
                  int         strategy);
      virtual ~ZlibEncoder();

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      virtual bool flush();

    protected:

      z_stream zstream_;
      Buffer   buffer_;
    };

    class DeflateEncoder : public ZlibEncoder
    {
    public:
      DeflateEncoder(Stream::Ptr stream,
                     int         level       = Z_DEFAULT_COMPRESSION,
                     int         method      = Z_DEFLATED,
                     int         window_bits = 15,
                     int         mem_level   = 8,
                     int         strategy    = Z_DEFAULT_STRATEGY);
    };

    class GzipEncoder : public ZlibEncoder
    {
    public:
      GzipEncoder(Stream::Ptr stream,
                  int         level       = Z_DEFAULT_COMPRESSION,
                  int         method      = Z_DEFLATED,
                  int         window_bits = 15 + 16,
                  int         mem_level   = 8,
                  int         strategy    = Z_DEFAULT_STRATEGY);
    };
  }
}

