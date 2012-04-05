#include <sys/sendfile.h>

#include <cerrno>
#include <algorithm>

#include "direct-fd-stream.hh"
#include "copy.hh"

namespace mimosa
{
  namespace stream
  {
    DirectFdStream::DirectFdStream(int fd, bool own_fd)
      : fd_(fd),
        own_fd_(own_fd),
        mode_(0)
    {
    }

    DirectFdStream::~DirectFdStream()
    {
      if (own_fd_ && fd_ >= 0)
      {
        ::close(fd_);
        fd_ = -1;
      }
    }

    int64_t
    DirectFdStream::write(const char * data, uint64_t nbytes, runtime::Time /*timeout*/)
    {
      return ::write(fd_, data, nbytes);
    }

    int64_t
    DirectFdStream::writev(const struct iovec *iov, int iovcnt, runtime::Time /*timeout*/)
    {
      return ::writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX);
    }

    int64_t
    DirectFdStream::read(char * data, uint64_t nbytes, runtime::Time /*timeout*/)
    {
      return ::read(fd_, data, nbytes);
    }

    int64_t
    DirectFdStream::readv(const struct iovec *iov, int iovcnt, runtime::Time /*timeout*/)
    {
      return ::readv(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX);
    }

    void
    DirectFdStream::close()
    {
      assert(own_fd_);
      if (own_fd_ && fd_ >= 0)
      {
        int fd = fd_;
        fd_    = -1;
        ::close(fd);
      }
    }

    bool
    DirectFdStream::stat() const
    {
      struct ::stat st;
      if (::fstat(fd_, &st))
        return false;
      mode_ = st.st_mode;
      return true;
    }

    int64_t copySendfile(DirectFdStream & input,
                         DirectFdStream & output,
                         int64_t          max_bytes,
                         runtime::Time    /*timeout*/)
    {
      uint64_t total = 0;

      while (total < max_bytes || max_bytes == 0) {
        int64_t limit;

        if (max_bytes == 0)
          limit = 128 * 1024;
        else
          limit = std::min((uint64_t)128 * 1024, (uint64_t)max_bytes - total);

        int64_t bytes = ::sendfile(input.fd(), output.fd(), nullptr, limit);
        if (bytes < 0)
          return total;
        total += bytes;
      }
      return total;
    }

    int64_t copy(DirectFdStream & input,
                 DirectFdStream & output,
                 int64_t          max_bytes,
                 runtime::Time    timeout)
    {
      if (S_ISREG(input.fdMode()))
        return copySendfile(input, output, max_bytes, timeout);
      // else if (S_ISFIFO(input.fdMode()) || S_ISSOCKET(input.fdMode()))
      //   return copySplice(input, output, max_bytes, timeout);
      return copy(static_cast<Stream &> (input), static_cast<Stream &> (output),
                  max_bytes, timeout);
    }
  }
}
