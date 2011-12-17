#include <sstream>
#include "response.hh"
#include "log.hh"

namespace mimosa
{
  namespace http
  {
    Response::Response()
      : status_(kStatusOk),
        keep_alive_(false),
        content_encoding_(kCodingIdentity),
        transfer_encoding_(kCodingIdentity),
        content_length_(-1),
        content_type_("text/plain"),
        cookies_(),
        unparsed_headers_()
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
        MIMOSA_LOG(Warning, http_log, "invalid Transfer-Encoding: %d", transfer_encoding_);
        break;
      }

      for (auto it = cookies_.begin(); it != cookies_.end(); ++it)
      {
        os << "Set-Cookie: " << it->key() << "=" << it->value();
        if (it->isSecure())
          os << "; Secure";
        if (it->isHttpOnly())
          os << "; HttpOnly";
        if (!it->domain().empty())
          os << "; Domain=" << it->domain();
        if (!it->expires().empty())
          os << "; Expires=" << it->expires();
        if (!it->path().empty())
          os << "; Path=" << it->path();
        os << "\r\n";
      }

      for (auto it = unparsed_headers_.begin(); it != unparsed_headers_.end(); ++it)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
          os << it->first << ": " << *it2 << "\r\n";

      os << "\r\n";
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
      cookies_.clear();
      unparsed_headers_.clear();
    }
  }
}
