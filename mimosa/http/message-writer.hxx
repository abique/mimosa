#include <cassert>
#include <cerrno>

#include "chunked-stream.hh"
#include "../format/print.hh"
#include "../stream/zlib-encoder.hh"
#include "log.hh"

namespace mimosa
{
  namespace http
  {
    template <typename Channel, typename Message>
    MessageWriter<Channel, Message>::MessageWriter(Channel & channel)
      : channel_(channel),
        buffers_(),
        header_sent_(false)
    {
    }

    template <typename Channel, typename Message>
    MessageWriter<Channel, Message>::~MessageWriter()
    {
      while (!buffers_.empty())
      {
        auto buf = buffers_.front();
        buffers_.pop();
        delete buf;
      }
    }

    template <typename Channel, typename Message>
    int64_t
    MessageWriter<Channel, Message>::read(char *, uint64_t)
    {
      assert(false && "read is an invalid operation on ResponseWriter");
      errno = EINVAL;
      return -1;
    }

    template <typename Channel, typename Message>
    int64_t
    MessageWriter<Channel, Message>::write(const char * data, uint64_t nbytes)
    {
      if (!header_sent_ && !sendHeader())
        return -1;

      return stream_->write(data, nbytes);
    }

    template <typename Channel, typename Message>
    bool
    MessageWriter<Channel, Message>::flush()
    {
      if (!stream_)
        return true;
      return stream_->flush();
    }

    template <typename Channel, typename Message>
    bool
    MessageWriter<Channel, Message>::finish()
    {
      if (!header_sent_ && !sendHeader())
          return false;

      if ((Message::content_encoding_ != kCodingIdentity ||
           Message::transfer_encoding_ != kCodingIdentity) &&
          !stream_->flush())
        return false;

      // Chunked-Encoding: write the final chunk
      if (Message::transfer_encoding_ == kCodingChunked &&
          channel_.stream_->loopWrite("0\r\n\r\n", 5) != 5)
        return false;
      return channel_.stream_->flush();
    }

    template <typename Channel, typename Message>
    bool
    MessageWriter<Channel, Message>::sendHeader()
    {
      // don't send the header two times
      if (header_sent_)
        return true;
      header_sent_ = true;

      // check if we have to use Chunked-Endcoding
      if (Message::content_length_ < 0 ||
          Message::transfer_encoding_ == kCodingChunked) {
        Message::transfer_encoding_ = kCodingChunked;
        stream_ = new ChunkedStream(channel_.stream_.get());
        // prevent to many small writes
        stream_ = new stream::BufferedStream(stream_);
      } else
        stream_ = channel_.stream_;

      // check if we have to encode the content
      switch (Message::content_encoding_) {
      case kCodingIdentity:
        break;

      case kCodingDeflate:
        stream_ = new stream::DeflateEncoder(stream_);
        break;

      case kCodingGzip:
        stream_ = new stream::GzipEncoder(stream_);
        break;

      default:
        http_log->error("unsupported content_encoding: %v",
                        Message::content_encoding_);
        break;
      }

      return Message::print(*channel_.stream_);
    }

    template <typename Channel, typename Message>
    void
    MessageWriter<Channel, Message>::clear()
    {
      Response::clear();
      header_sent_ = false;
      stream_      = nullptr;
    }

    template <typename Channel, typename Message>
    stream::DirectFdStream *
    MessageWriter<Channel, Message>::directFdStream()
    {
      if (!header_sent_ ||
          Message::transfer_encoding_ != kCodingIdentity ||
          Message::content_encoding_ != kCodingIdentity)
        return NULL;

      stream::Stream * stream = channel_.stream_->underlyingStream();
      stream::DirectFdStream * fd_stream = dynamic_cast<stream::DirectFdStream *> (stream);
      if (!fd_stream)
        return NULL;
      if (!channel_.stream_->flush())
        return NULL;
      return fd_stream;
    }

    template <typename Channel, typename Message>
    bool
    MessageWriter<Channel, Message>::enableCompression(const Request & request)
    {
      if (Message::content_encoding_ != kCodingIdentity)
        return true;

      if (header_sent_)
        return false;

      if (request.acceptEncoding() & kCodingGzip) {
        Message::content_encoding_ = kCodingGzip;
        return true;
      } else if (request.acceptEncoding() & kCodingDeflate) {
        Message::content_encoding_ = kCodingDeflate;
        return true;
      }

      return false;
    }
  }
}
