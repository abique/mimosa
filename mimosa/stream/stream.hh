#ifndef MIMOSA_STREAM_STREAM_HH
# define MIMOSA_STREAM_STREAM_HH

# include <sys/uio.h>

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

      /** flushes the write buffer */
      virtual bool flush(runtime::Time timeout) {}
    };
  }
}

#endif /* !MIMOSA_STREAM_STREAM_HH */
