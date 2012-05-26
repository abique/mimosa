#include <cassert>
#include <cerrno>

#include "response-writer.hh"
#include "server-channel.hh"
#include "../format/print.hh"

namespace mimosa
{
  namespace http
  {
    ResponseWriter::ResponseWriter(ServerChannel & channel,
                                   Time   write_timeout)
      : channel_(channel),
        buffers_(),
        header_sent_(false),
        write_timeout_(write_timeout)
    {
    }

    ResponseWriter::~ResponseWriter()
    {
      while (!buffers_.empty())
      {
        auto buf = buffers_.front();
        buffers_.pop();
        delete buf;
      }
    }

    int64_t
    ResponseWriter::read(char *, uint64_t, Time)
    {
      assert(false && "read is an invalid operation on ResponseWriter");
      errno = EINVAL;
      return -1;
    }

    int64_t
    ResponseWriter::writeChunk(const char * data,
                               uint64_t     nbytes,
                               Time         timeout)
    {
      bool ok = true;

      ok = ok & format::printHex(*channel_.stream_, nbytes, timeout);
      ok = ok & format::printStatic(*channel_.stream_, "\r\n", timeout);
      ok = ok & format::print(*channel_.stream_, data, nbytes, timeout);
      ok = ok & format::printStatic(*channel_.stream_, "\r\n", timeout);
      return ok ? nbytes : -1;
    }

    int64_t
    ResponseWriter::write(const char * data, uint64_t nbytes, Time timeout)
    {
      if (!header_sent_ && !sendHeader(timeout))
        return -1;

      if (transfer_encoding_ == kCodingChunked)
        return writeChunk(data, nbytes, timeout);
      else
        return channel_.stream_->loopWrite(data, nbytes, timeout);
    }

    bool
    ResponseWriter::flush(Time timeout)
    {
      return channel_.stream_->flush(timeout);
    }

    bool
    ResponseWriter::finish(Time timeout)
    {
      if (!header_sent_ && !sendHeader(timeout))
          return false;

      // write the final chunk
      if (transfer_encoding_ == kCodingChunked && writeChunk("", 0, timeout) < 0)
          return false;
      return true;
    }

    bool
    ResponseWriter::sendHeader(Time timeout)
    {
      if (header_sent_)
        return true;
      header_sent_ = true;
      if (content_length_ < 0)
        transfer_encoding_ = kCodingChunked;
      return print(*channel_.stream_, timeout);
    }

    void
    ResponseWriter::clear()
    {
      Response::clear();
      header_sent_ = false;
    }

    stream::DirectFdStream *
    ResponseWriter::directFdStream()
    {
      if (!header_sent_)
        return NULL;

      stream::Stream * stream = channel_.stream_->underlyingStream();
      stream::DirectFdStream * fd_stream = dynamic_cast<stream::DirectFdStream *> (stream);
      if (!fd_stream)
        return NULL;
      if (!channel_.stream_->flush(write_timeout_))
        return NULL;
      return fd_stream;
    }
  }
}
