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
      virtual ~FdStream();

      virtual bool atEnd() const;

      virtual uint64_t bytesAvailable() const;
      virtual uint64_t bytesToWrite() const;

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);
      virtual void flush();

      virtual int64_t pos() const;
      virtual bool seek(int64_t offset, SeekType whence);

    private:
      int fd_;
    };
  }
}

#endif /* !MIMOSA_STREAM_FD_STREAM_HH */
