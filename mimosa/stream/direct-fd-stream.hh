#ifndef MIMOSA_STREAM_DIRECT_FD_STREAM_HH
# define MIMOSA_STREAM_DIRECT_FD_STREAM_HH

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class DirectFdStream : public Stream
    {
    public:
      MIMOSA_DEF_PTR(DirectFdStream);

      DirectFdStream(int fd, bool own_fd = true);
      virtual ~DirectFdStream();

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t writev(const struct iovec *iov, int iovcnt);
      virtual int64_t read(char * data, uint64_t nbytes);
      virtual int64_t readv(const struct iovec *iov, int iovcnt);

      virtual void close();

      int fd() const { return fd_; }
      ::mode_t fdMode() const { if (!mode_) stat(); return mode_; }
      bool stat() const;

    private:
      int      fd_;
      bool     own_fd_;
      mutable ::mode_t mode_; // st_mode from struct ::stat
    };

    int64_t copy(DirectFdStream & input,
                 DirectFdStream & output,
                 int64_t          max_bytes = 0);
  }
}

#endif /* !MIMOSA_STREAM_DIRECT_FD_STREAM_HH */
