#include <cassert>
#include <cerrno>

#include "response-writer.hh"
#include "server-channel.hh"
#include "chunked-stream.hh"
#include "../format/print.hh"
#include "../stream/zlib-encoder.hh"
#include "log.hh"

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
    ResponseWriter::write(const char * data, uint64_t nbytes)
    {
      if (!header_sent_ && !sendHeader())
        return -1;

      return stream_->write(data, nbytes);
    }

    bool
    ResponseWriter::flush()
    {
      if (!stream_)
        return true;
      return stream_->flush();
    }

    bool
    ResponseWriter::finish()
    {
      if (!header_sent_ && !sendHeader())
          return false;

      if ((content_encoding_ != kCodingIdentity ||
           transfer_encoding_ != kCodingIdentity) &&
          !stream_->flush())
        return false;

      // Chunked-Encoding: write the final chunk
      if (transfer_encoding_ == kCodingChunked &&
          channel_.stream_->loopWrite("0\r\n\r\n", 5) != 5)
        return false;
      return channel_.stream_->flush();
    }

    bool
    ResponseWriter::sendHeader()
    {
      // don't send the header two times
      if (header_sent_)
        return true;
      header_sent_ = true;

      // check if we have to use Chunked-Endcoding
      if (content_length_ < 0 || transfer_encoding_ == kCodingChunked) {
        transfer_encoding_ = kCodingChunked;
        stream_ = new ChunkedStream(channel_.stream_);
        // prevent to many small writes
        stream_ = new stream::BufferedStream(stream_);
      } else
        stream_ = channel_.stream_;

      // check if we have to encode the content
      switch (content_encoding_) {
      case kCodingIdentity:
        break;

      case kCodingDeflate:
        stream_ = new stream::ZlibEncoder(stream_, 9, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
        break;

      case kCodingGzip:
        stream_ = new stream::ZlibEncoder(stream_, 9, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
        break;

        // XXX add more encoders (gzip, etc..)

      default:
        http_log->error("unsupported content_encoding: %v", content_encoding_);
        break;
      }

      return print(*channel_.stream_);
    }

    void
    ResponseWriter::clear()
    {
      Response::clear();
      header_sent_ = false;
      stream_      = nullptr;
    }

    stream::DirectFdStream *
    ResponseWriter::directFdStream()
    {
      if (!header_sent_ ||
          transfer_encoding_ != kCodingIdentity ||
          content_encoding_ != kCodingIdentity)
        return NULL;

      stream::Stream * stream = channel_.stream_->underlyingStream();
      stream::DirectFdStream * fd_stream = dynamic_cast<stream::DirectFdStream *> (stream);
      if (!fd_stream)
        return NULL;
      if (!channel_.stream_->flush())
        return NULL;
      return fd_stream;
    }

    bool
    ResponseWriter::enableCompression(const Request & request)
    {
      if (content_encoding_ != kCodingIdentity)
        return true;

      if (header_sent_)
        return false;

      // XXX add support for other encodings
      if (request.acceptEncoding() & kCodingGzip) {
        content_encoding_ = kCodingGzip;
        return true;
      } else if (request.acceptEncoding() & kCodingDeflate) {
        content_encoding_ = kCodingDeflate;
        return true;
      }

      return false;
    }
  }
}
