#ifndef MIMOSA_STREAM_COMPRESS_HH
# define MIMOSA_STREAM_COMPRESS_HH

# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class Compress : public Filter
    {
    public:
      MIMOSA_DEF_PTR(Compress);

      Compress(Stream::Ptr stream, int level = 4);

      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0);

      virtual void close();
      virtual bool flush(Time timeout = 0);

    private:
      int         level_;
    };
  }
}

#endif /* !MIMOSA_STREAM_COMPRESS_HH */
