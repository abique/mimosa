#include "fd-stream.hh"
#include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    FdStream::FdStream(int fd, uint64_t buffer_size, bool is_readable, bool is_writable)
      : BufferedStream(new DirectFdStream(fd, is_readable, is_writable), buffer_size)
    {
    }

    FdStream::FdStream(DirectFdStream::Ptr stream, uint64_t buffer_size)
      : BufferedStream(stream, buffer_size)
    {
    }

    FdStream::Ptr
    FdStream::openFile(const char * path, int oflags, mode_t mode)
    {
      auto stream = DirectFdStream::openFile(path, oflags, mode);
      return new FdStream(stream);
    }
  }
}
