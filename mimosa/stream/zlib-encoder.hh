#ifndef MIMOSA_STREAM_ZLIB_ENCODER_HH
# define MIMOSA_STREAM_ZLIB_ENCODER_HH

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
  }
}

#endif /* !MIMOSA_STREAM_ZLIB_ENCODER_HH */
