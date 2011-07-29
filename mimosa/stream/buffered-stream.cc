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
        rbuffer_(nullptr),
        rpos_(0),
        rappend_(0)
    {
    }

    BufferedStream::~BufferedStream()
    {
      while (!wbuffers_.empty())
      {
        auto tmp = &wbuffers_.front();
        wbuffers_.pop();
        delete tmp;
      }
      delete rbuffer_;
      rbuffer_ = nullptr;
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
        rpos_    = 0;
        rappend_ = 0;
        int64_t rbytes = stream_->read(rbuffer_->data(), rbuffer_->size(), timeout);
        if (rbytes <= 0)
          return 0;
        rappend_ = rbytes;
      }

      assert(rappend_ > rpos_);
      assert(rbuffer_);
      nbytes = nbytes <= rappend_ - rpos_ ? nbytes : rappend_ - rpos_;
      ::memcpy(data, rbuffer_->data(), nbytes);
      rpos_ += nbytes;
      return nbytes;
    }
  }
}
