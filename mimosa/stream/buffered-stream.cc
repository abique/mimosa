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
        wbuffers_(),
        wpos_(0),
        wappend_(0),
        rbuffer_(),
        rpos_(0),
        rappend_(0)
    {
    }

    BufferedStream::~BufferedStream()
    {
    }

    inline
    uint64_t
    BufferedStream::readyWrite() const
    {
      return buffer_size_ - wpos_ + wappend_ +
        (wbuffers_.size() > 2 ? buffer_size_ * (wbuffers_.size() - 2) : 0);
    }

    void
    BufferedStream::append(const char * data, uint64_t nbytes)
    {
      assert(nbytes < buffer_size_);
      if (wbuffers_.empty())
      {
        wbuffers_.push(*new Buffer(buffer_size_));
        wpos_    = 0;
        wappend_ = 0;
      }

      auto * buffer = &wbuffers_.back();
      uint64_t can_write = nbytes <= buffer_size_ - wappend_ ? nbytes : buffer_size_ - wappend_;
      ::memcpy(buffer->data() + wappend_, data, can_write);
      if (can_write < nbytes)
      {
        wappend_ = nbytes - can_write;
        buffer = new Buffer(buffer_size_);
        ::memcpy(buffer->data(), data + can_write, wappend_);
        wbuffers_.push(*buffer);
      }
      else
        wappend_ += can_write;
    }

    int64_t
    BufferedStream::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      const auto nbytes_orig = nbytes;

      // while we have more than buffer_size_ to write
      while (nbytes + readyWrite() >= buffer_size_)
      {
        // setup iov
        const  int     iov_cnt = wbuffers_.size() + 1;
        struct iovec   iov[iov_cnt];
        struct iovec * p = iov;

        for (auto it = wbuffers_.begin(); it != wbuffers_.end(); ++it, ++p)
        {
          p->iov_base = it->data();
          p->iov_len  = buffer_size_;
          if (p == iov)
          {
            p->iov_base += wpos_;
            p->iov_len  -= wpos_;
          }
          if (p == iov + iov_cnt - 1)
            p->iov_len -= wappend_;
        }
        iov[iov_cnt - 1].iov_base = const_cast<char *>(data);
        iov[iov_cnt - 1].iov_len  = nbytes;

        // write and remove completed buffers
        int64_t wbytes = stream_->writev(iov, iov_cnt, timeout);
        if (wbytes < 0)
        {
          if (nbytes_orig == nbytes)
            return -1;
          return nbytes_orig - nbytes;
        }

        // remove completed buffers
        p = iov;
        for (auto it = wbuffers_.begin(); it != wbuffers_.end(); ++it, ++p)
        {
          if (wbytes >= p->iov_len)
          {
            wbytes -= p->iov_len;
            wbuffers_.pop();
          }
          else
          {
            wpos_ = wbytes;
            wbytes = 0;
            break;
          }
        }
        nbytes -= wbytes;
      }

      if (nbytes > 0)
        append(data, nbytes);
      return nbytes_orig;
    }

    int64_t
    BufferedStream::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (rappend_ == rpos_)
      {
        if (!rbuffer_)
          rbuffer_ = new Buffer(buffer_size_);
        else if (rbuffer_->size() != buffer_size_)
          rbuffer_->resize(buffer_size_);
        rpos_    = 0;
        rappend_ = 0;
        int64_t rbytes = stream_->read(rbuffer_->data(), rbuffer_->size(), timeout);
        if (rbytes <= 0)
          return rbytes;
        rappend_ = rbytes;
      }

      assert(rappend_ > rpos_);
      assert(rbuffer_);
      nbytes = nbytes <= rappend_ - rpos_ ? nbytes : rappend_ - rpos_;
      ::memcpy(data, rbuffer_->data(), nbytes);
      rpos_ += nbytes;
      if (rpos_ == rappend_)
      {
        rpos_    = 0;
        rappend_ = 0;
      }
      return nbytes;
    }

    Buffer::Ptr
    BufferedStream::read(uint64_t buffer_size, runtime::Time timeout)
    {
      if (rappend_ > rpos_)
      {
        assert(!rbuffer_);
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
      int64_t bytes = stream_->read(rbuffer_->data(), rbuffer_->size(), timeout);
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
                              runtime::Time      timeout)
    {
      Buffer::Ptr buffer;
      size_t      str_len = strlen(str);

      while (true)
      {
        if (max_size > 0 && buffer->size() > max_size)
        {
          errno = EFBIG;
          return nullptr;
        }

        Buffer::Ptr tmp_buffer = read(timeout);
        if (!tmp_buffer || tmp_buffer->size() == 0)
        {
          assert(!rbuffer_);
          rbuffer_ = buffer;
          rappend_ = buffer->size();
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

        int64_t offset = buffer->size() - tmp_buffer->size() - str_len + 1;
        const char * found = ::strstr(buffer->data() + (offset > 0 ? offset : 0), str);
        if (found)
        {
          assert(!rbuffer_);
          rbuffer_ = new Buffer(buffer_size_);
          const int64_t buffer_size = found + str_len - buffer->data();
          rappend_ = buffer->size() - buffer_size;
          memcpy(rbuffer_->data(), buffer->data() + buffer_size, rappend_);
          buffer->resize(buffer_size_);
          return buffer;
        }
      }
    }

    bool
    BufferedStream::flush(runtime::Time timeout)
    {
      assert(false);
      return true;
    }
  }
}
