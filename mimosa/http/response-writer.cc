#include <cassert>
#include <cerrno>

#include "response-writer.hh"
#include "server-channel.hh"

namespace mimosa
{
  namespace http
  {
    ResponseWriter::ResponseWriter(ServerChannel & channel,
                                   runtime::Time   write_timeout)
      : channel_(channel),
        buffers_(),
        header_sent_(false),
        write_timeout_(write_timeout)
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
    ResponseWriter::writeChunk(const char *  data,
                               uint64_t      nbytes,
                               runtime::Time timeout)
    {
      char buffer[32];
      auto bytes = snprintf(buffer, sizeof (buffer), "%lX\r\n", nbytes);

      if (channel_.stream_->loopWrite(buffer, bytes, timeout) != bytes)
        return -1;
      if (channel_.stream_->loopWrite(data, nbytes, timeout) != nbytes)
        return -1;
      if (channel_.stream_->loopWrite("\r\n", 2, timeout) != 2)
        return -1;
      return nbytes;
    }

    int64_t
    ResponseWriter::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      if (header_sent_)
      {
        if (transfer_encoding_ == kCodingChunked)
          return writeChunk(data, nbytes, timeout);
        else
          return channel_.stream_->loopWrite(data, nbytes, timeout);
      }

      stream::Buffer::Ptr buffer = new stream::Buffer(data, nbytes);
      buffers_.push(buffer);
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
      assert(header_sent_);
      for (auto it = buffers_.begin(); it != buffers_.end(); ++it)
      {
        auto ret = write(it->data(), it->size(), timeout);
        if (ret < 0)
          return false;
      }
      buffers_.clear();
      return true;
    }

    bool
    ResponseWriter::finish(runtime::Time timeout)
    {
      if (!header_sent_)
      {
        transfer_encoding_ = kCodingIdentity;
        content_length_ = pendingWrite();
        if (!sendHeader(timeout))
          return false;
      }

      // write the final chunk
      if (transfer_encoding_ == kCodingChunked)
        if (writeChunk("", 0, timeout) < 0)
          return false;
      return channel_.stream_->flush(timeout);
    }

    bool
    ResponseWriter::sendHeader(runtime::Time timeout)
    {
      if (header_sent_)
        return true;
      header_sent_ = true;
      if (content_length_ < 0)
        transfer_encoding_ = kCodingChunked;
      auto data = toHttpHeader();
      return channel_.stream_->loopWrite(data.data(), data.size(), timeout) ==
        static_cast<int64_t> (data.size()) &&
        flush();
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
