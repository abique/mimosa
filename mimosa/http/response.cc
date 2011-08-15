#include "response.hh"

namespace mimosa
{
  namespace http
  {
    bool
    Response::hasBody() const
    {
      return body_;
    }

    std::string &&
    Response::toHttpHeader() const
    {
      std::ostringstream os;
      os << "HTTP/1.1 " << status_ << "\r\n"
         << "Server: mimosa\r\n"
         << "Connection: " << (keep_alive_ ? "Keep-Alive" : "Close") << "\r\n";
      if (content_length_ > 0)
        os << "Content-Length: " << content_length_ << "\r\n";
      for (auto it = cookies_.begin(); it != cookies_.end(); ++it)
        ;
      os << "\r\n";
    }
  }
}
