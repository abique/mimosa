#ifndef MIMOSA_STREAM_STREAM_HH
# define MIMOSA_STREAM_STREAM_HH

# include <sys/types.h>
# include <sys/uio.h>
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
