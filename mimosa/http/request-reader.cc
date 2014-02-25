#include "request-reader.hh"
#include "server-channel.hh"
#include "message-reader.hxx"

namespace mimosa
{
  namespace http
  {
    template class MessageReader<ServerChannel, Request>;
  }
}
