#include <sstream>
#include "response.hh"
#include "log.hh"
#include "time.hh"

namespace mimosa
{
  namespace http
  {
    Response::Response()
    {
      clear(); // to ensure the same state
    }

    std::string
    Response::toHttpHeader() const
    {
      std::ostringstream os;
      os << "HTTP/1.1 " << status_ << " " << statusToString(status_) <<"\r\n"
         << "Server: mimosa\r\n"
         << "Connection: " << (keep_alive_ ? "Keep-Alive" : "Close") << "\r\n";
      if (content_length_ >= 0)
        os << "Content-Length: " << content_length_ << "\r\n";
      if (!content_type_.empty())
        os << "Content-Type: " << content_type_ << "\r\n";

      switch (transfer_encoding_)
      {
      case kCodingIdentity:
        os << "Transfer-Encoding: Identity\r\n";
        break;

      case kCodingChunked:
        os << "Transfer-Encoding: Chunked\r\n";
        break;

      default:
        http_log->warning("invalid Transfer-Encoding: %d", transfer_encoding_);
        break;
      }

      for (auto it = cookies_.begin(); it != cookies_.end(); ++it)
      {
        os << "Set-Cookie: " << it->key() << "=" << it->value();
        if (!it->domain().empty())
          os << "; Domain=" << it->domain();
        if (!it->path().empty())
          os << "; Path=" << it->path();
        if (!it->expires().empty())
          os << "; Expires=" << it->expires();
        if (it->isSecure())
          os << "; Secure";
        if (it->isHttpOnly())
          os << "; HttpOnly";
        os << "\r\n";
      }

      for (auto it = unparsed_headers_.begin(); it != unparsed_headers_.end(); ++it)
        os << it->first << ": " << it->second << "\r\n";

      if (last_modified_ > 0)
        os << "Last-Modified: " << http::time(last_modified_) << " GMT\r\n";

      os << "\r\n"; // end of response

      return os.str();
    }

    void
    Response::clear()
    {
      status_            = kStatusOk;
      keep_alive_        = false;
      content_encoding_  = kCodingIdentity;
      transfer_encoding_ = kCodingIdentity;
      content_length_    = -1;
      content_type_      = "text/plain";
      last_modified_     = 0;
      cookies_.clear();
      unparsed_headers_.clear();
    }
  }
}
