#pragma once

# include "buffered-stream.hh"
# include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    class FdStream : public BufferedStream
    {
    public:
      MIMOSA_DEF_PTR(FdStream);

      explicit FdStream(int fd, uint64_t buffer_size = 64 * 1024, bool own_fd = true);
      explicit FdStream(const DirectFdStream::Ptr& stream, uint64_t buffer_size = 64 * 1024);

      static FdStream::Ptr openFile(const char * path, int oflags = O_RDONLY, mode_t mode = 0644);
    };
  }
}

