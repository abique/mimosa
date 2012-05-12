#ifndef MIMOSA_STREAM_NET_FD_STREAM_HH
# define MIMOSA_STREAM_NET_FD_STREAM_HH

# include "direct-fd-stream.hh"

namespace mimosa
{
  namespace stream
  {
    class NetFdStream : public DirectFdStream
    {
    public:
      MIMOSA_DEF_PTR(NetFdStream);

      NetFdStream(int fd, bool own_fd);

      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0);
      virtual int64_t writev(const struct iovec *iov, int iovcnt, Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0);
      virtual int64_t readv(const struct iovec *iov, int iovcnt, Time timeout = 0);
    };
  }
}

#endif /* !MIMOSA_STREAM_NET_FD_STREAM_HH */
