#include "request-writer.hh"
#include "client-channel.hh"
#include "message-writer.hxx"

namespace mimosa
{
  namespace http
  {
    template class MessageWriter<ClientChannel, Request>;

    ResponseReader::Ptr
    RequestWriter::send()
    {
      if (!channel_.connect(
            url_.host(), url_.port(), url_.scheme() == uri::kSchemeHttps))
        return nullptr;
      if (!sendHeader())
        return nullptr;

      ResponseReader::Ptr rr = new ResponseReader(channel_);
      // XXX parse response headers
      return rr;
    }
  }
}
