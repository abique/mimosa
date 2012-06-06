#include <cassert>
#include <cstring>

#include "buffered-stream.hh"

namespace mimosa
{
  namespace stream
  {
    BufferedStream::BufferedStream(Stream::Ptr stream, uint64_t buffer_size)
      : stream_(stream),
        buffer_size_(buffer_size),
        wbuffer_(buffer_size_),
        wpos_(0),
        rbuffer_(),
        rpos_(0),
        rappend_(0)
    {
    }

    BufferedStream::~BufferedStream()
    {
    }

    uint64_t
    BufferedStream::readyRead() const
    {
      return rappend_ - rpos_;
    }

    uint64_t
    BufferedStream::readyWrite() const
    {
      return wpos_;
    }

    int64_t
    BufferedStream::write(const char * data, uint64_t nbytes)
    {
      if (wpos_ + nbytes >= buffer_size_)
      {
        if (flushWith(data, nbytes))
          return nbytes;
        return -1;
      }

      ::memcpy(wbuffer_.data() + wpos_, data, nbytes);
      wpos_ += nbytes;
      return nbytes;
    }

    bool
    BufferedStream::flush()
    {
      if (stream_->loopWrite(wbuffer_.data(), wpos_) != static_cast<int64_t> (wpos_))
        return false;
      wpos_ = 0;
      return stream_->flush();
    }

    bool
    BufferedStream::flushWith(const char *data, uint64_t nbytes)
    {
      if (wpos_ == 0)
        return stream_->loopWrite(data, nbytes) == static_cast<int64_t> (nbytes);

      struct ::iovec iov[2];

      iov[0].iov_base = wbuffer_.data();
      iov[0].iov_len = wpos_;
      iov[1].iov_base = (void*)data;
      iov[1].iov_len = nbytes;

      bool succeed = stream_->loopWritev(iov, 2);

      if (succeed)
        wpos_ = 0;

      return succeed;
    }

    int64_t
    BufferedStream::read(char * data, uint64_t nbytes)
    {
      if (rappend_ == rpos_)
      {
        if (!rbuffer_)
          rbuffer_ = new Buffer(buffer_size_);
        else if (rbuffer_->size() != buffer_size_)
          rbuffer_->resize(buffer_size_);
        rpos_    = 0;
        rappend_ = 0;
        int64_t rbytes = stream_->read(rbuffer_->data(), rbuffer_->size());
        if (rbytes <= 0)
          return rbytes;
        rappend_ = rbytes;
      }

      assert(rappend_ > rpos_);
      assert(rbuffer_);
      nbytes = nbytes <= rappend_ - rpos_ ? nbytes : rappend_ - rpos_;
      ::memcpy(data, rbuffer_->data() + rpos_, nbytes);
      rpos_ += nbytes;
      if (rpos_ == rappend_)
      {
        rpos_    = 0;
        rappend_ = 0;
      }
      return nbytes;
    }

    Buffer::Ptr
    BufferedStream::read(uint64_t buffer_size)
    {
      if (rappend_ > rpos_)
      {
        assert(rbuffer_);
        ::memmove(rbuffer_->data(), rbuffer_->data() + rpos_, rappend_ - rpos_);
        rbuffer_->resize(rappend_ - rpos_);
        auto tmp = rbuffer_;
        rbuffer_ = nullptr;
        rappend_ = 0;
        rpos_ = 0;
        return tmp;
      }

      if (!rbuffer_)
        rbuffer_ = new Buffer(buffer_size);
      int64_t bytes = stream_->read(rbuffer_->data(), rbuffer_->size());
      if (bytes <= 0)
        return nullptr;
      rbuffer_->resize(bytes);
      auto tmp = rbuffer_;
      rbuffer_ = nullptr;
      return tmp;
    }

    Buffer::Ptr
    BufferedStream::readUntil(const char * const str,
                              uint64_t           max_size, // TODO
                              bool *             found)
    {
      Buffer::Ptr buffer;
      size_t      str_len = strlen(str);

      if (found)
        *found = false;

      while (true)
      {
        if (max_size > 0 && buffer && buffer->size() > max_size)
        {
          errno = EFBIG;
          return nullptr;
        }

        Buffer::Ptr tmp_buffer = read(buffer_size_);
        if (!tmp_buffer || tmp_buffer->size() == 0)
        {
          assert(!rbuffer_ || rappend_ == 0);
          rbuffer_ = buffer;
          rappend_ = buffer ? buffer->size() : 0;
          return nullptr;
        }

        if (buffer)
        {
          buffer->resize(buffer->size() + tmp_buffer->size());
          memcpy(buffer->data() + buffer->size() - tmp_buffer->size(),
                 tmp_buffer->data(), tmp_buffer->size());
        }
        else
          buffer = tmp_buffer;

        int64_t offset = buffer->size() - tmp_buffer->size() - str_len;
        const char * found_str = ::strstr(buffer->data() + (offset > 0 ? offset : 0), str);
        if (found_str)
        {
          assert(!rbuffer_);
          if (found)
            *found = true;

          /* setup the remaining read buffer */
          rbuffer_ = new Buffer(buffer_size_);
          const int64_t remain_size = found_str + str_len - buffer->data();
          rappend_ = buffer->size() - remain_size;
          memcpy(rbuffer_->data(), buffer->data() + remain_size, rappend_);

          /* resize the return buffer */
          buffer->resize(found_str - buffer->data() + str_len);
          return buffer;
        }
      }
    }

    void
    BufferedStream::close()
    {
      stream_->close();
    }
  }
}
