#include <sstream>

#include "../string-ref.hh"
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

    Response::~Response()
    {
      clear();
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
      if (hasContentRange()) {
        ok = ok & format::printStatic(stream, "Content-Range: bytes ");
        ok = ok & format::printDecimal(stream, content_range_start_);
        ok = ok & format::printStatic(stream, "-");
        ok = ok & format::printDecimal(stream, content_range_end_);
        ok = ok & format::printStatic(stream, "/");
        ok = ok & format::printDecimal(stream, content_range_length_);
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
        ok = ok & format::printStatic(stream, "\r\n");
      }

      if (!location_.empty()) {
        ok = ok & format::printStatic(stream, "Location: ");
        ok = ok & format::print(stream, location_);
        ok = ok & format::printStatic(stream, "\r\n");
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
      content_type_      = "";
      last_modified_     = 0;
      proto_major_       = 1;
      proto_minor_       = 1;

      while (!cookies_.empty()) {
        auto cookie = cookies_.front();
        cookies_.pop();
        delete cookie;
      }
      unparsed_headers_.clear();
      location_.clear();
    }

    bool
    Response::parse(const char * data, size_t size)
    {
      StringRef in(data, size);

      clear();
      StringRef line = in.consumeLine("\r\n");
      if (!parseStatus(line))
        return false;

      while (!in.empty()) {
        line = in.consumeLine("\r\n");

        // check end of headers
        if (line == "\r\n")
            return true;

        parseHeader(line);
      }
      return true;
    }

    bool
    Response::parseStatus(StringRef & line)
    {
      line.eatWhitespaces(" \t");
      auto token_http = line.consumeToken(" \t");
      if (!token_http.strncaseeq("http/1."))
        return false;

      proto_major_ = 1;
      proto_minor_ = 1;

      line.eatWhitespaces(" \t");
      auto token_status = line.consumeToken(" \t");
      status_ = token_status.atoi<int>();
      return true;
    }

    bool
    Response::parseHeader(StringRef & line)
    {
      line.eatWhitespaces(" \t");
      auto key = line.consumeToken(" :\t");
      line.eatWhitespaces(" \t");
      if (line.empty() || line[0] != ':')
        return false;
      line = line.substr(1);
      line.eatWhitespaces(" \t");
      auto value = line.substr(0, line.size() - 2);

      if (key.strcaseeq("Content-Length"))
        content_length_ = value.atoi<uint64_t>();
      else if (key.strcaseeq("Connection")) {
        if (value.strcaseeq("Keep-Alive"))
          keep_alive_ = true;
        else if (value.strcaseeq("Close"))
          keep_alive_ = false;
        else
          keep_alive_ = proto_minor_;
      } else if (key.strcaseeq("Content-Type"))
        content_type_ = value;
      else if (key.strcaseeq("Content-Encoding")) {
        if (value.strcaseeq("compress"))
          content_encoding_ = kCodingCompress;
        else if (value.strcaseeq("identify"))
          content_encoding_ = kCodingIdentity;
        else if (value.strcaseeq("deflate"))
          content_encoding_ = kCodingDeflate;
        else if (value.strcaseeq("gzip"))
          content_encoding_ = kCodingGzip;
        else if (value.strcaseeq("sdch"))
          content_encoding_ = kCodingSdch;
      } else if (key.strcaseeq("Transfer-Encoding")) {
        if (value.strcaseeq("chunked"))
          transfer_encoding_ = kCodingChunked;
        else if (value.strcaseeq("identity"))
          transfer_encoding_ = kCodingIdentity;
      } else
        unparsed_headers_.emplace(std::make_pair(key, value));
      return true;
    }
  }
}
