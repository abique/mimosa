#include <sstream>
#include "response.hh"
#include "log.hh"
#include "time.hh"
#include "../format/print.hh"
#include "../stream/string-stream.hh"

namespace mimosa
{
  namespace http
  {
    Response::Response()
    {
      clear(); // to ensure the same state
    }

    bool
    Response::print(stream::Stream & stream, Time timeout) const
    {
      bool ok = true;
      ok = ok & format::printStatic(stream, "HTTP/1.1 ", timeout);
      ok = ok & format::printDecimal(stream, static_cast<int> (status_), timeout);
      ok = ok & format::printStatic(stream, " ", timeout);
      ok = ok & format::print(stream, statusToString(status_), timeout);
      ok = ok & format::printStatic(stream, "\r\nServer: mimosa\r\nConnection: ", timeout);
      if (keep_alive_)
        ok = ok & format::printStatic(stream, "Keep-Alive\r\n", timeout);
      else
        ok = ok & format::printStatic(stream, "Close\r\n", timeout);
      if (content_length_ >= 0)
      {
        ok = ok & format::printStatic(stream, "Content-Length: ", timeout);
        ok = ok & format::printDecimal(stream, content_length_, timeout);
        ok = ok & format::printStatic(stream, "\r\n", timeout);
      }
      if (!content_type_.empty())
      {
        ok = ok & format::printStatic(stream, "Content-Type: ", timeout);
        ok = ok & format::print(stream, content_type_, timeout);
        ok = ok & format::printStatic(stream, "\r\n", timeout);
      }

      switch (transfer_encoding_)
      {
      case kCodingIdentity:
        ok = ok & format::printStatic(stream, "Transfer-Encoding: Identity\r\n", timeout);
        break;

      case kCodingChunked:
        ok = ok & format::printStatic(stream, "Transfer-Encoding: Chunked\r\n", timeout);
        break;

      default:
        http_log->warning("invalid Transfer-Encoding: %d", transfer_encoding_);
        break;
      }

      for (auto it = cookies_.begin(); it != cookies_.end(); ++it)
      {
        ok = ok & format::printStatic(stream, "Set-Cookie: ", timeout);
        ok = ok & format::print(stream, it->key(), timeout);
        ok = ok & format::printStatic(stream, "=", timeout);
        ok = ok & format::print(stream, it->value(), timeout);
        if (!it->domain().empty())
        {
          ok = ok & format::printStatic(stream, "; Domain=", timeout);
          ok = ok & format::print(stream, it->domain(), timeout);
        }
        if (!it->path().empty())
        {
          ok = ok & format::printStatic(stream, "; Path=", timeout);
          ok = ok & format::print(stream, it->path(), timeout);
        }
        if (!it->expires().empty())
        {
          ok = ok & format::printStatic(stream, "; Expires=", timeout);
          ok = ok & format::print(stream, it->expires(), timeout);
        }
        if (it->isSecure())
          ok = ok & format::printStatic(stream, "; Secure", timeout);
        if (!it->isHttpOnly())
          ok = ok & format::printStatic(stream, "; HttpOnly", timeout);
        ok = ok & format::printStatic(stream, "\r\n", timeout);
      }

      for (auto it = unparsed_headers_.begin(); it != unparsed_headers_.end(); ++it)
      {
        ok = ok & format::print(stream, it->first, timeout);
        ok = ok & format::printStatic(stream, ": ", timeout);
        ok = ok & format::print(stream, it->second, timeout);
        ok = ok & format::printStatic(stream, "\r\n", timeout);
      }

      if (last_modified_ > 0)
      {
        ok = ok & format::printStatic(stream, "Last-Modified: ", timeout);
        ok = ok & format::print(stream, http::time(last_modified_), timeout);
        ok = ok & format::printStatic(stream, " GMT\r\n", timeout);
      }

      // end of response
      ok = ok & format::printStatic(stream, "\r\n", timeout);
      return ok;
    }

    std::string
    Response::toHttpHeader() const
    {
      stream::StringStream stream;

      if (!print(stream, 0))
        return "(error)";
      return stream.str();
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
