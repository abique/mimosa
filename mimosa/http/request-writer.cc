#include "request-writer.hh"
#include "client-channel.hh"
#include "message-writer.hxx"

namespace mimosa
{
  namespace http
  {
    template class MessageWriter<ClientChannel, Request>;
  }
}
