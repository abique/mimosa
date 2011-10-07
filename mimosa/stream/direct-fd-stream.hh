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

      DirectFdStream(int fd, bool own_fd = true);
      virtual ~DirectFdStream();

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t writev(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t readv(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);

      virtual void close();

      int fd() const { return fd_; }

    private:
      int  fd_;
      bool own_fd_;
    };
  }
}

#endif /* !MIMOSA_STREAM_DIRECT_FD_STREAM_HH */
