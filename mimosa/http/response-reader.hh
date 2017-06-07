#pragma once

# include "response.hh"
# include "message-reader.hh"

namespace mimosa
{
  namespace http
  {
    class ClientChannel;
    class Request;

    typedef MessageReader<ClientChannel, Response> ResponseReader;
    extern template class MessageReader<ClientChannel, Response>;
  }
}

