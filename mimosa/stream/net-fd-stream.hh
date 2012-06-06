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

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t writev(const struct iovec *iov, int iovcnt);
      virtual int64_t read(char * data, uint64_t nbytes);
      virtual int64_t readv(const struct iovec *iov, int iovcnt);

      /**
       * @param timeout an absolute point in the monotonic clock reference
       * after which, every operation will fails with errno set to ETIMEDOUT.
       * If timeout = 0, then it will never timeout.
       */
      inline void setTimeout(Time timeout) { timeout_ = timeout; }
      inline Time timeout() const { return timeout_; }

    private:
      Time timeout_;
    };
  }
}

#endif /* !MIMOSA_STREAM_NET_FD_STREAM_HH */
