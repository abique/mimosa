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

    uint64_t
    BufferedStream::readyRead() const
    {
      return rappend_ - rpos_;
    }

    uint64_t
    BufferedStream::readyWrite() const
    {
      int      i     = 0;
      int      nb    = wbuffers_.size();
      uint64_t bytes = 0;

      for (auto it = wbuffers_.begin(); it != wbuffers_.end(); ++it, ++i)
      {
        bytes += it->size();
        if (i == 0)
          bytes -= wpos_;
        if (i == nb - 1)
          bytes -= it->size() - wappend_;
      }
      return bytes;
    }

    void
    BufferedStream::append(const char * data, uint64_t nbytes)
    {
      assert(nbytes < buffer_size_);
      if (wbuffers_.empty())
      {
        wbuffers_.push(new Buffer(buffer_size_));
        wpos_    = 0;
        wappend_ = 0;
      }

      Buffer::Ptr buffer = wbuffers_.back();
      uint64_t can_write = nbytes <= buffer_size_ - wappend_ ? nbytes : buffer_size_ - wappend_;
      ::memcpy(buffer->data() + wappend_, data, can_write);
      if (can_write < nbytes)
      {
        wappend_ = nbytes - can_write;
        buffer = new Buffer(buffer_size_ > wappend_ ? buffer_size_ : wappend_);
        ::memcpy(buffer->data(), data + can_write, wappend_);
        wbuffers_.push(buffer);
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
          p->iov_len  = it->size();
          if (p == iov)
          {
            p->iov_base = static_cast<uint8_t*>(p->iov_base) + wpos_;
            p->iov_len  -= wpos_;
          }
          if (p == iov + wbuffers_.size() - 1)
            p->iov_len -= it->size() - wappend_;
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
          if (wbytes >= static_cast<int64_t> (p->iov_len))
          {
            wbytes -= p->iov_len;
            wbuffers_.pop();
            wpos_ = 0;
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
    BufferedStream::read(uint64_t buffer_size, runtime::Time timeout)
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
                              runtime::Time      timeout,
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

        Buffer::Ptr tmp_buffer = read(buffer_size_, timeout);
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

    bool
    BufferedStream::flush(runtime::Time timeout)
    {
      while (!wbuffers_.empty())
      {
        // setup iov
        const  int           iov_cnt = wbuffers_.size();
        struct iovec         iov[iov_cnt];
        struct iovec * const iov_last = iov + iov_cnt - 1;
        struct iovec *       p        = iov;

        // adjust the first and last buffer
        for (auto it = wbuffers_.begin(); it != wbuffers_.end(); ++it, ++p)
        {
          p->iov_base = it->data();
          p->iov_len  = it->size();
          if (p == iov)
          {
            p->iov_base = static_cast<uint8_t*> (p->iov_base) + wpos_;
            p->iov_len  -= wpos_;
          }
          if (p == iov_last)
            p->iov_len -= (it->size() - wappend_);
        }

        // write and remove completed buffers
        int64_t wbytes = stream_->writev(iov, iov_cnt, timeout);
        if (wbytes < 0)
          return false;

        // remove completed buffers
        p = iov;
        while (!wbuffers_.empty())
        {
          auto it = wbuffers_.begin();

          if (wbytes < static_cast<int64_t> (p->iov_len))
          {
            wpos_ += wbytes;
            break;
          }

          wbytes -= p->iov_len;
          wbuffers_.pop();
          wpos_ = 0;
          ++p;
        }
      }

      return stream_->flush(timeout);
    }

    void
    BufferedStream::close()
    {
      stream_->close();
    }
  }
}
