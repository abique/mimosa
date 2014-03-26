#include "request-writer.hh"
#include "client-channel.hh"
#include "message-writer.hxx"

namespace mimosa
{
  namespace http
  {
    template class MessageWriter<ClientChannel, Request>;

    bool
    RequestWriter::sendRequest()
    {
      bool is_ssl = url_.scheme() == uri::kSchemeHttps;

      if (!channel_.connect(url_.host(), url_.port(), is_ssl))
        return false;

      if (!sendHeader())
        return false;
      return true;
    }

    ResponseReader::Ptr
    RequestWriter::response()
    {
      if (!flush())
        return nullptr;

      ResponseReader::Ptr rr = new ResponseReader(channel_);

      bool found = false;
      stream::Buffer::Ptr buffer = channel_.stream_->readUntil(
        "\r\n\r\n", 5 * 1024, &found);
      if (!buffer || !found || !rr->parse(buffer->data(), buffer->size()))
        return nullptr;
      if (!rr->prepare())
        return nullptr;
      return rr;
    }

    ResponseReader::Ptr
    RequestWriter::send()
    {
      if (!sendRequest())
        return nullptr;
      return response();
    }
  }
}
