#ifndef MIMOSA_STREAM_STREAM_HH
# define MIMOSA_STREAM_STREAM_HH

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

      inline Stream(bool is_readable, bool is_writable, bool is_sequential)
        : is_readable_(is_readable),
          is_writable_(is_writable),
          timeout_(0)
      {
      }

      inline virtual ~Stream() {}

      inline bool isReadable() const { return is_readable_; }
      inline bool isWritable() const { return is_writable_; }

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0) = 0;
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0) = 0;
      /** flushes the write buffer */
      virtual bool flush(runtime::Time timeout) = 0;

    private:
      unsigned is_readable_ : 1;
      unsigned is_writable_ : 1;
      unsigned is_sequential_ : 1;
    };
  }
}

#endif /* !MIMOSA_STREAM_STREAM_HH */
