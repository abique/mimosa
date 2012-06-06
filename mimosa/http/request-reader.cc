#include <cstring>

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
        bytes_left_(0),
        parsed_form_(false),
        form_()
    {
    }

    void
    RequestReader::clear()
    {
      bytes_left_ = 0;
      parsed_form_ = false;
      form_.clear();
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
      uint64_t can_read = static_cast<int64_t>(nbytes) <= bytes_left_ ? nbytes : bytes_left_;
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

    kvs &
    RequestReader::form()
    {
      if (parsed_form_ ||
          ::strcasecmp(contentType().c_str(), "application/x-www-form-urlencoded"))
        return form_;

      parsed_form_ = true;
      stream::Buffer buffer(contentLength());
      int64_t rbytes = 0;

      rbytes = channel_.stream_->loopRead(buffer.data(), contentLength());
      if (rbytes < contentLength())
        return form_;

      uri::parseQuery(buffer.data(), buffer.size(), &form_);
      return form_;
    }
  }
}
