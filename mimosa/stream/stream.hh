#ifndef MIMOSA_STREAM_STREAM_HH
# define MIMOSA_STREAM_STREAM_HH

# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <limits.h>

# include "../ref-countable.hh"
# include "../non-copyable.hh"
# include "../runtime/time.hh"

namespace mimosa
{
  namespace stream
  {
    class Stream : public RefCountable<Stream>,
                   private NonCopyable
    {
    public:
      inline virtual ~Stream() {}

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0) = 0;
      virtual int64_t writev(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0) = 0;
      virtual int64_t readv(const struct iovec *iov, int iovcnt, runtime::Time timeout = 0);

      virtual void close() {}

      /** flushes the write buffer */
      inline virtual bool flush(runtime::Time /*timeout*/) { return true; }

      int64_t loopRead(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      int64_t loopWrite(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
    };
  }
}

#endif /* !MIMOSA_STREAM_STREAM_HH */
