#include <cassert>
#include <cerrno>

#include "response-writer.hh"
#include "server-channel.hh"
#include "chunked-stream.hh"
#include "../format/print.hh"
#include "../stream/zlib-encoder.hh"
#include "log.hh"
#include "message-writer.hxx"

namespace mimosa
{
  namespace http
  {
    template class MessageWriter<ServerChannel, Response>;
  }
}
