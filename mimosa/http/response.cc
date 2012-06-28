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
    Response::print(stream::Stream & stream) const
    {
      bool ok = true;
      ok = ok & format::printStatic(stream, "HTTP/1.1 ");
      ok = ok & format::printDecimal(stream, static_cast<int> (status_));
      ok = ok & format::printStatic(stream, " ");
      ok = ok & format::print(stream, statusToString(status_));
      ok = ok & format::printStatic(stream, "\r\nServer: mimosa\r\nConnection: ");
      if (keep_alive_)
        ok = ok & format::printStatic(stream, "Keep-Alive\r\n");
      else
        ok = ok & format::printStatic(stream, "Close\r\n");
      if (content_length_ >= 0)
      {
        ok = ok & format::printStatic(stream, "Content-Length: ");
        ok = ok & format::printDecimal(stream, content_length_);
        ok = ok & format::printStatic(stream, "\r\n");
      }
      if (!content_type_.empty())
      {
        ok = ok & format::printStatic(stream, "Content-Type: ");
        ok = ok & format::print(stream, content_type_);
        ok = ok & format::printStatic(stream, "\r\n");
      }

      switch (content_encoding_)
      {
      case kCodingIdentity:
        break;

      case kCodingDeflate:
        ok = ok & format::printStatic(stream, "Content-Encoding: deflate\r\n");
        break;

      case kCodingGzip:
        ok = ok & format::printStatic(stream, "Content-Encoding: gzip\r\n");
        break;

      case kCodingCompress:
        ok = ok & format::printStatic(stream, "Content-Encoding: compress\r\n");
        break;

      case kCodingSdch:
        ok = ok & format::printStatic(stream, "Content-Encoding: sdch\r\n");
        break;

      default:
        http_log->warning("invalid Content-Encoding: %d", content_encoding_);
        break;
      }

      switch (transfer_encoding_)
      {
      case kCodingIdentity:
        // don't tell that the transfer encoding is identity as some old
        // http clients could fails...
        //ok = ok & format::printStatic(stream, "Transfer-Encoding: Identity\r\n");
        break;

      case kCodingChunked:
        ok = ok & format::printStatic(stream, "Transfer-Encoding: Chunked\r\n");
        break;

      default:
        http_log->warning("invalid Transfer-Encoding: %d", transfer_encoding_);
        break;
      }

      for (auto it = cookies_.begin(); it != cookies_.end(); ++it)
      {
        ok = ok & format::printStatic(stream, "Set-Cookie: ");
        ok = ok & format::print(stream, it->key());
        ok = ok & format::printStatic(stream, "=");
        ok = ok & format::print(stream, it->value());
        if (!it->domain().empty())
        {
          ok = ok & format::printStatic(stream, "; Domain=");
          ok = ok & format::print(stream, it->domain());
        }
        if (!it->path().empty())
        {
          ok = ok & format::printStatic(stream, "; Path=");
          ok = ok & format::print(stream, it->path());
        }
        if (!it->expires().empty())
        {
          ok = ok & format::printStatic(stream, "; Expires=");
          ok = ok & format::print(stream, it->expires());
        }
        if (it->isSecure())
          ok = ok & format::printStatic(stream, "; Secure");
        if (it->isHttpOnly())
          ok = ok & format::printStatic(stream, "; HttpOnly");
        ok = ok & format::printStatic(stream, "\r\n");
      }

      for (auto it = unparsed_headers_.begin(); it != unparsed_headers_.end(); ++it)
      {
        ok = ok & format::print(stream, it->first);
        ok = ok & format::printStatic(stream, ": ");
        ok = ok & format::print(stream, it->second);
        ok = ok & format::printStatic(stream, "\r\n");
      }

      if (last_modified_ > 0)
      {
        ok = ok & format::printStatic(stream, "Last-Modified: ");
        ok = ok & format::print(stream, http::time(last_modified_));
        ok = ok & format::printStatic(stream, " GMT\r\n");
      }

      // end of response
      ok = ok & format::printStatic(stream, "\r\n");
      return ok;
    }

    std::string
    Response::toHttpHeader() const
    {
      stream::StringStream stream;

      if (!print(stream))
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
