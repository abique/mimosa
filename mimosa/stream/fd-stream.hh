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

    private:
      int fd_;
    };
  }
}

#endif /* !MIMOSA_STREAM_FD_STREAM_HH */
