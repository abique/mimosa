#ifndef MIMOSA_STREAM_FD_STREAM_HH
# define MIMOSA_STREAM_FD_STREAM_HH

# include "buffered-stream.hh"

namespace mimosa
{
  namespace stream
  {
    class FdStream : public BufferedStream
    {
    public:
      MIMOSA_DEF_PTR(FdStream);

      FdStream(int fd, uint64_t buffer_size = 64 * 1024, bool is_readable = 1, bool is_writable = 1);
    };
  }
}

#endif /* !MIMOSA_STREAM_FD_STREAM_HH */
