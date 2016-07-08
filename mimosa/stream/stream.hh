#ifndef MIMOSA_STREAM_STREAM_HH
# define MIMOSA_STREAM_STREAM_HH

# include "../compat/uio.hh"

# include <unistd.h>
# include <limits.h>

# include <string>

# include "../ref-countable.hh"
# include "../non-copyable.hh"
# include "../time.hh"

namespace mimosa
{
  namespace stream
  {
    class Stream : public RefCountable<Stream>,
                   private NonCopyable
    {
    public:
      inline virtual ~Stream() {}

      virtual int64_t write(const char * data, uint64_t nbytes) = 0;
      virtual int64_t writev(const struct iovec *iov, int iovcnt);
      virtual int64_t read(char * data, uint64_t nbytes) = 0;
      virtual int64_t readv(const struct iovec *iov, int iovcnt);

      /**
       * @param timeout an absolute point in the monotonic clock reference
       * after which, every operation will fails with errno set to ETIMEDOUT.
       * If timeout = 0, then it will never timeout.
       * @{
       */
      virtual void setReadTimeout(Time timeout) { (void)timeout; }
      virtual void setWriteTimeout(Time timeout) { (void)timeout; }

      inline void setTimeout(Time timeout) {
        setReadTimeout(timeout);
        setWriteTimeout(timeout);
      }
      /** @} */

      /**
       * @param timeout a duration after which a blocking I/O fails.
       * @warning This setting only apply if the thread is blocked on
       * I/O. If combined with setTimeout(), it may not work as expected
       * as the previous one may block in (select, poll, epoll_wait).
       * @{
       */
      virtual void setReadIdleTimeout(Time timeout) { (void)timeout; }
      virtual void setWriteIdleTimeout(Time timeout) { (void)timeout; }

      inline void setIdleTimeout(Time timeout) {
        setReadIdleTimeout(timeout);
        setWriteIdleTimeout(timeout);
      }
      /** @} */

      /** helper */
      inline int64_t write(const std::string & str) {
        return write(str.data(), str.size());
      }

      /** cancels running io opperations */
      inline virtual void close() {}

      /** flushes the write buffer */
      inline virtual bool flush() { return true; }

      int64_t loopRead(char * data, uint64_t nbytes);
      // XXX loopReadv
      int64_t loopWrite(const char * data, uint64_t nbytes);
      int64_t loopWritev(struct iovec *iov, int iovcnt);
    };
  }
}

#endif /* !MIMOSA_STREAM_STREAM_HH */
