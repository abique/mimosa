#include "fd-stream.hh"
#include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    FdStream::FdStream(int fd, uint64_t buffer_size, bool own_fd)
      : BufferedStream(new DirectFdStream(fd, own_fd), buffer_size)
    {
    }

    FdStream::FdStream(DirectFdStream::Ptr stream, uint64_t buffer_size)
      : BufferedStream(stream, buffer_size)
    {
    }

    FdStream::Ptr
    FdStream::openFile(const char * path, int oflags, mode_t mode)
    {
      int fd = ::open(path, oflags, mode);
      if (fd < 0)
        return nullptr;

      auto stream = new DirectFdStream(fd, true);
      return new FdStream(stream);
    }
  }
}
