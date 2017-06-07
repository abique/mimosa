#pragma once

# include "request.hh"
# include "message-reader.hh"

namespace mimosa
{
  namespace http
  {
    class  ServerChannel;
    typedef MessageReader<ServerChannel, Request> RequestReader;
    extern template class MessageReader<ServerChannel, Request>;
  }
}

