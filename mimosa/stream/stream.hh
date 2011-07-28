#ifndef MIMOSA_STREAM_STREAM_HH
# define MIMOSA_STREAM_STREAM_HH

namespace mimosa
{
  namespace stream
  {
    class Stream : public RefCountable<Stream>
    {
    public:

      inline Stream(bool is_readable, bool is_writable, bool is_sequential)
        : is_readable_(is_readable),
          is_writable_(is_writable),
          is_sequential_(is_sequential),
          timeout_(0)
      {
      }

      inline virtual ~Stream() {}

      enum SeekType
      {
        kSeekCur,
        kSeekSet,
        kSeekEnd,
      }

      inline bool isReadable() const { return is_readable_; }
      inline bool isWritable() const { return is_writable_; }
      inline bool isSequential() const { return is_sequential_; }

      virtual bool atEnd() const = 0;

      virtual uint64_t bytesAvailable() const = 0;
      virtual uint64_t bytesToWrite() const = 0;

      virtual int64_t write(const char * data, uint64_t nbytes) = 0;
      virtual int64_t read(char * data, uint64_t nbytes) = 0;
      virtual void flush() = 0;

      virtual int64_t pos() const = 0;
      virtual bool seek(int64_t offset, SeekType whence) = 0;

      inline bool getChar(char * c) { return 1 == read(c, 1); }
      inline bool putChar(char c) { return 1 == write(&c, 1); }

      inline void setTimeout(runtime::Time timeout) { timeout_ = timeout; }
      inline Time timeout() const { return timeout_; }

    private:
      unsigned is_readable_ : 1;
      unsigned is_writeable_ : 1;
      unsigned is_sequential_ : 1;
      runtime::Time timeout_;
    };
  }
}

#endif /* !MIMOSA_STREAM_STREAM_HH */
