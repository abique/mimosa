#include <cstring>
#include <algorithm>

#include "../uri/parse-query.hh"
#include "../uri/percent-encoding.hh"
#include "../stream/buffer.hh"
#include "request-reader.hh"
#include "server-channel.hh"

namespace mimosa
{
  namespace http
  {
    RequestReader::RequestReader(ServerChannel & channel)
      : channel_(channel),
        bytes_left_(0)
    {
    }

    void
    RequestReader::clear()
    {
      bytes_left_ = 0;
    }

    bool
    RequestReader::prepare()
    {
      if (contentLength() < 0)
        return false;
      bytes_left_ = contentLength();
      return true;
    }

    int64_t
    RequestReader::write(const char * /*data*/, uint64_t /*nbytes*/)
    {
      assert(false && "invalid operation");
      return -1;
    }

    int64_t
    RequestReader::read(char * data, uint64_t nbytes)
    {
      uint64_t can_read = std::min(nbytes, static_cast<uint64_t> (bytes_left_));
      int64_t rbytes = channel_.stream_->read(data, can_read);
      if (rbytes > 0)
        bytes_left_ -= rbytes;
      return rbytes;
    }

    bool
    RequestReader::flush()
    {
      if (bytes_left_ <= 0)
        return true;

      if (parsed_form_ && bytes_left_ > 0)
        return false;

      stream::Buffer buffer(bytes_left_);
      while (bytes_left_ > 0)
        if (read(buffer.data(), buffer.size()) <= 0)
          return false;
      return true;
    }
  }
}
