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
      {
        os << "Set-Cookie: " << it->key() << "=" << value->value();
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
      os << "\r\n";
    }
  }
}
