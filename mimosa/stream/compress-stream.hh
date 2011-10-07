#ifndef MIMOSA_STREAM_COMPRESS_STREAM_HH
# define MIMOSA_STREAM_COMPRESS_STREAM_HH

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class CompressStream : public Stream
    {
    public:
      CompressStream(Stream::Ptr stream, int level = 4);

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);

      virtual void close();
      virtual bool flush(runtime::Time timeout = 0);

    private:
      Stream::Ptr stream_;
      int         level_;
    };
  }
}

#endif /* !MIMOSA_STREAM_COMPRESS_STREAM_HH */
