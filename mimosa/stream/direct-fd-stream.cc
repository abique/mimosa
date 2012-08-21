#include <sys/sendfile.h>

#include <cerrno>
#include <algorithm>
#include <limits>

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
    DirectFdStream::write(const char * data, uint64_t nbytes)
    {
      return ::write(fd_, data, nbytes);
    }

    int64_t
    DirectFdStream::writev(const struct iovec *iov, int iovcnt)
    {
      return ::writev(fd_, iov, iovcnt < IOV_MAX ? iovcnt : IOV_MAX);
    }

    int64_t
    DirectFdStream::read(char * data, uint64_t nbytes)
    {
      return ::read(fd_, data, nbytes);
    }

    int64_t
    DirectFdStream::readv(const struct iovec *iov, int iovcnt)
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

    static int64_t copySendfile(DirectFdStream & input,
                                DirectFdStream & output,
                                int64_t          max_bytes)
    {
      int64_t total = 0;

      while (total < max_bytes || max_bytes == 0) {
        int64_t limit;

        if (max_bytes == 0)
          limit = std::numeric_limits<int64_t>::max();
        else
          limit = max_bytes - total;

        ssize_t bytes = ::sendfile(output.fd(), input.fd(), nullptr, limit);
        if (bytes <= 0) {
          if (errno == EAGAIN)
            continue;
          return total;
        }
        total += bytes;
      }
      return total;
    }

    static int64_t copySplice(DirectFdStream & input,
                              DirectFdStream & output,
                              int64_t          max_bytes)
    {
      int64_t total = 0;

      while (total < max_bytes || max_bytes == 0) {
        int64_t limit;

        if (max_bytes == 0)
          limit = std::numeric_limits<int64_t>::max();
        else
          limit = max_bytes - total;

        ssize_t bytes = ::splice(input.fd(), nullptr, output.fd(), nullptr, limit, 0);
        if (bytes <= 0) {
          if (errno == EAGAIN)
            continue;
          return total;
        }
        total += bytes;
      }
      return total;
    }

#if 0
    static int64_t copySpliceAny(DirectFdStream & input,
                                 DirectFdStream & output,
                                 int64_t          max_bytes)
    {
      int     pfd[2];
      int64_t total = 0;

      if (::pipe2(pfd, O_CLOEXEC | O_NONBLOCK))
        return -1;

      while (total < max_bytes || max_bytes == 0) {
        int64_t limit;

        if (max_bytes == 0)
          limit = std::numeric_limits<int64_t>::max();
        else
          limit = max_bytes - total;

        ssize_t rbytes = ::splice(input.fd(), nullptr, pfd[1], nullptr, limit, 0);
        if (bytes <= 0) {
          if (errno == EAGAIN)
            continue;
          goto end;
        }

        while (rbytes > 0) {
          ssize_t wbytes = ::splice(pfd[0], nullptr, output.fd(), nullptr, rbytes, 0);
          if (wbytes < 0) {
            if (errno == EAGAIN)
              continue;
            total = -1;
            goto end;
          }

          total  += wbytes;
          rbytes -= wbytes;
        }
      }

      end:
      ::close(pfd[0]);
      ::close(pfd[1]);
      return total;
    }
#endif

    int64_t copy(DirectFdStream & input,
                 DirectFdStream & output,
                 int64_t          max_bytes)
    {
      // disk to fd
      if (S_ISREG(input.fdMode()))
        return copySendfile(input, output, max_bytes);

      // from a pipe or to a pipe
      if (S_ISFIFO(input.fdMode()) || S_ISFIFO(output.fdMode()))
        return copySplice(input, output, max_bytes);

#if 0
      // splice: input -> pipe -> output
      if (max_bytes == 0 || max_bytes > 4096)
        return copySpliceAny(input, output, max_bytes);
#endif

      // user copy
      return copy(static_cast<Stream &> (input),
                  static_cast<Stream &> (output),
                  max_bytes);
    }
  }
}
