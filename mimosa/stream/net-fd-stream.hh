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

      NetFdStream(int fd, bool own_fd = true);

      virtual int64_t write(const char * data, uint64_t nbytes) override;
      virtual int64_t writev(const struct iovec *iov, int iovcnt) override;
      virtual int64_t read(char * data, uint64_t nbytes) override;
      virtual int64_t readv(const struct iovec *iov, int iovcnt) override;

      virtual void setReadTimeout(Time timeout) override;
      virtual void setWriteTimeout(Time timeout) override;

    private:
      Time read_timeout_;
      Time write_timeout_;
    };
  }
}

#endif /* !MIMOSA_STREAM_NET_FD_STREAM_HH */
