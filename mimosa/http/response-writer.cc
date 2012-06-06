#include <cassert>
#include <cerrno>

#include "response-writer.hh"
#include "server-channel.hh"
#include "../format/print.hh"

namespace mimosa
{
  namespace http
  {
    ResponseWriter::ResponseWriter(ServerChannel & channel)
      : channel_(channel),
        buffers_(),
        header_sent_(false)
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
    ResponseWriter::read(char *, uint64_t)
    {
      assert(false && "read is an invalid operation on ResponseWriter");
      errno = EINVAL;
      return -1;
    }

    int64_t
    ResponseWriter::writeChunk(const char * data,
                               uint64_t     nbytes)
    {
      bool ok = true;

      ok = ok & format::printHex(*channel_.stream_, nbytes);
      ok = ok & format::printStatic(*channel_.stream_, "\r\n");
      ok = ok & format::print(*channel_.stream_, data, nbytes);
      ok = ok & format::printStatic(*channel_.stream_, "\r\n");
      return ok ? nbytes : -1;
    }

    int64_t
    ResponseWriter::write(const char * data, uint64_t nbytes)
    {
      if (!header_sent_ && !sendHeader())
        return -1;

      if (transfer_encoding_ == kCodingChunked)
        return writeChunk(data, nbytes);
      else
        return channel_.stream_->loopWrite(data, nbytes);
    }

    bool
    ResponseWriter::flush()
    {
      return channel_.stream_->flush();
    }

    bool
    ResponseWriter::finish()
    {
      if (!header_sent_ && !sendHeader())
          return false;

      // write the final chunk
      if (transfer_encoding_ == kCodingChunked && writeChunk("", 0) < 0)
          return false;
      return true;
    }

    bool
    ResponseWriter::sendHeader()
    {
      if (header_sent_)
        return true;
      header_sent_ = true;
      if (content_length_ < 0)
        transfer_encoding_ = kCodingChunked;
      return print(*channel_.stream_);
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
      if (!channel_.stream_->flush())
        return NULL;
      return fd_stream;
    }
  }
}
