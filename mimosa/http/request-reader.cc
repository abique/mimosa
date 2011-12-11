#include <cstring>

#include "../uri/parse-query.hh"
#include "../uri/percent-encoding.hh"
#include "../stream/buffer.hh"
#include "request-reader.hh"

namespace mimosa
{
  namespace http
  {
    RequestReader::RequestReader(stream::Stream::Ptr stream,
                                 runtime::Time       read_timeout)
      : stream_(stream),
        bytes_left_(0),
        parsed_form_(false),
        form_(),
        read_timeout_(read_timeout)
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
    RequestReader::write(const char * /*data*/, uint64_t /*nbytes*/, runtime::Time /*timeout*/)
    {
      assert(false && "invalid operation");
      return -1;
    }

    int64_t
    RequestReader::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      uint64_t can_read = nbytes <= bytes_left_ ? nbytes : bytes_left_;
      int64_t rbytes = stream_->read(data, can_read, timeout);
      if (rbytes > 0)
        bytes_left_ -= rbytes;
      return rbytes;
    }

    bool
    RequestReader::flush(runtime::Time timeout)
    {
      stream::Buffer buffer;
      while (bytes_left_ > 0)
        if (read(buffer.data(), buffer.size(), timeout) <= 0)
          return false;
      return true;
    }

    container::kvs &
    RequestReader::form()
    {
      if (parsed_form_)
        return form_;

      if (!::strcasecmp(contentType().c_str(), "application/x-www-form-urlencoded"))
      {
        parsed_form_ = true;
        stream::Buffer buffer(contentLength());
        int64_t total_rbytes = 0;

        while (total_rbytes < contentLength())
        {
          auto rbytes = stream_->read(buffer.data(), contentLength() - total_rbytes, 0);
          if (rbytes <= 0)
            return form_;
          total_rbytes += rbytes;
        }

        std::string decoded;
        uri::percentDecode(buffer.data(), buffer.size(), &decoded);
        uri::parseQuery(decoded.data(), decoded.size(), &form_);
      }

      return form_;
    }
  }
}
