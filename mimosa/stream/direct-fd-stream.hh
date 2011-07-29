#ifndef MIMOSA_STREAM_DIRECT_FD_STREAM_HH
# define MIMOSA_STREAM_DIRECT_FD_STREAM_HH

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class DirectFdStream : public Stream
    {
    public:
      DirectFdStream(int fd, bool is_readable = 1, bool is_writable = 1);
      virtual ~DirectFdStream();

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t writev(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t readv(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);

      int fd() const { return fd_; }

    private:
      int fd_;
      int is_readable_ : 1;
      int is_writable_ : 1;
    };
  }
}

#endif /* !MIMOSA_STREAM_DIRECT_FD_STREAM_HH */
