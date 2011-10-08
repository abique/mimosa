#include "../stream/fd-stream.hh"
#include "server.hh"
#include "channel.hh"

namespace mimosa
{
  namespace rpc
  {
    Server::Server()
    {
    }

    void
    Server::serve(int fd) const
    {
      stream::FdStream::Ptr stream = new stream::FdStream(fd);
      Channel::Ptr channel = new Channel(stream, service_map_);
      channel->start();
    }
  }
}
