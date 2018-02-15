#pragma once

# include "request.hh"
# include "message-writer.hh"
# include "response-reader.hh"

namespace mimosa
{
  namespace http
  {
    class ClientChannel;

    class RequestWriter : public MessageWriter<ClientChannel, Request>
    {
    public:
      MIMOSA_DEF_PTR(RequestWriter);

      inline explicit RequestWriter(ClientChannel & channel)
        : MessageWriter<ClientChannel, Request> (channel)
      {
      }

      bool sendRequest();
      ResponseReader::Ptr response();

      ResponseReader::Ptr send();
    };

    extern template class MessageWriter<ClientChannel, Request>;
  }
}

