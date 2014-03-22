#include <cstring>
#include <algorithm>
#include <limits>

#include "../uri/parse-query.hh"
#include "../uri/percent-encoding.hh"
#include "../stream/buffer.hh"
#include "chunked-stream.hh"

namespace mimosa
{
  namespace http
  {
    template <typename Channel, typename Message>
    MessageReader<Channel, Message>::MessageReader(Channel & channel)
      : channel_(channel),
        bytes_left_(0)
    {
    }

    template <typename Channel, typename Message>
    void
    MessageReader<Channel, Message>::clear()
    {
      bytes_left_ = 0;
    }

    template <typename Channel, typename Message>
    bool
    MessageReader<Channel, Message>::prepare()
    {
      if (Message::transferEncoding() == kCodingChunked)
        stream_ = new ChunkedStream(channel_.stream());
      else {
        if (Message::contentLength() < 0)
          return false;
        bytes_left_ = Message::contentLength();
        stream_ = channel_.stream();
      }
      return true;
    }

    template <typename Channel, typename Message>
    int64_t
    MessageReader<Channel, Message>::write(const char * /*data*/, uint64_t /*nbytes*/)
    {
      assert(false && "invalid operation");
      return -1;
    }

    template <typename Channel, typename Message>
    int64_t
    MessageReader<Channel, Message>::read(char * data, uint64_t nbytes)
    {
      uint64_t can_read = std::min(nbytes, static_cast<uint64_t> (bytes_left_));

      if (Message::transferEncoding() == kCodingChunked)
        can_read = nbytes;

      int64_t rbytes = stream_->read(data, can_read);
      if (rbytes > 0)
        bytes_left_ -= rbytes;
      return rbytes;
    }

    template <typename Channel, typename Message>
    bool
    MessageReader<Channel, Message>::flush()
    {
      if (bytes_left_ <= 0)
        return true;

      stream::Buffer buffer(bytes_left_);
      while (bytes_left_ > 0)
        if (read(buffer.data(), buffer.size()) <= 0)
          return false;
      return true;
    }
  }
}
