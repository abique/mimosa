#ifndef MIMOSA_STREAM_DIRECT_FD_STREAM_HH
# define MIMOSA_STREAM_DIRECT_FD_STREAM_HH

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class DirectFdStream : public Stream
    {
    public:
      MIMOSA_DEF_PTR(DirectFdStream);

      DirectFdStream(int fd, bool is_readable = 1, bool is_writable = 1, bool is_regular_file = false);
      virtual ~DirectFdStream();

      static DirectFdStream::Ptr openFile(const char * path, int oflags = O_RDONLY, mode_t mode = 0644);

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t writev(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t readv(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);

      int fd() const { return fd_; }

    private:
      int  fd_;
      bool is_readable_ : 1;
      bool is_writable_ : 1;
      bool is_regular_file_ : 1;
    };
  }
}

#endif /* !MIMOSA_STREAM_DIRECT_FD_STREAM_HH */
