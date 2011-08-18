#include <cassert>
#include <cerrno>

#include "response-writer.hh"

namespace mimosa
{
  namespace http
  {
    ResponseWriter::ResponseWriter(stream::FdStream::Ptr stream)
      : stream_(stream),
        buffers_(),
        header_sent_(false)
    {
    }

    ResponseWriter::~ResponseWriter()
    {
    }

    int64_t
    ResponseWriter::read(char *, uint64_t, runtime::Time)
    {
      assert(false && "read is an invalid operation on ResponseWriter");
      errno = EINVAL;
      return -1;
    }

    int64_t
    ResponseWriter::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (header_sent_)
        return stream_->write(data, nbytes, timeout);
      stream::Buffer::Ptr buffer = new stream::Buffer(data, nbytes);
      buffers_.push(*buffer);
      return nbytes;
    }

    uint64_t
    ResponseWriter::pendingWrite() const
    {
      uint64_t bytes = 0;
      for (auto it = buffers_.begin(); it != buffers_.end(); ++it)
        bytes += it->size();
      return bytes;
    }

    bool
    ResponseWriter::flush(runtime::Time timeout)
    {
      for (auto it = buffers_.begin(); it != buffers_.end(); ++it)
      {
        auto ret = stream_->write(it->data(), it->size(), timeout);
        if (ret < 0)
          return false;
        assert(ret == it->size()); // because of underlying buffered stream
                                   // and lazy programmer ;-)
      }
      buffers_.clear();
      stream_->flush();
      return true;
    }

    bool
    ResponseWriter::finish(runtime::Time timeout)
    {
      if (!header_sent_ && content_length_ == 0 && !buffers_.empty())
        for (auto it = buffers_.begin(); it != buffers_.end(); ++it)
          content_length_ += it->size();
      return sendHeader(timeout) && flush(timeout);
    }

    bool
    ResponseWriter::sendHeader(runtime::Time timeout)
    {
      if (header_sent_)
        return true;
      header_sent_ = true;
      auto data = toHttpHeader();
      return stream_->write(data.data(), data.size(), timeout) == data.size();
    }

    void
    ResponseWriter::clear()
    {
      Response::clear();
      buffers_.clear();
      header_sent_ = false;
    }
  }
}
