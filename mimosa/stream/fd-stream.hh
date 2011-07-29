#ifndef MIMOSA_STREAM_FD_STREAM_HH
# define MIMOSA_STREAM_FD_STREAM_HH

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class FdStream : public Stream
    {
    public:
      FdStream(int fd);
      virtual ~FdStream() override;

      virtual bool atEnd() const override;

      virtual uint64_t bytesAvailable() const override;
      virtual uint64_t bytesToWrite() const override;

      virtual int64_t write(const char * data, uint64_t nbytes) override;
      virtual int64_t read(char * data, uint64_t nbytes) override;
      virtual void flush() override;

      virtual int64_t pos() const override;
      virtual bool seek(int64_t offset, SeekType whence) override;

    private:
      int fd_;
    };
  }
}

#endif /* !MIMOSA_STREAM_FD_STREAM_HH */
