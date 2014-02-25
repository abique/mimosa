#include <cassert>
#include <cerrno>

#include "response-reader.hh"
#include "server-channel.hh"
#include "chunked-stream.hh"
#include "../format/print.hh"
#include "../stream/zlib-encoder.hh"
#include "log.hh"
#include "client-channel.hh"
#include "message-reader.hxx"

namespace mimosa
{
  namespace http
  {
    template class MessageReader<ClientChannel, Response>;
  }
}
