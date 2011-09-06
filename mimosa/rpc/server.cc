#include "../runtime/fiber.hh"
#include "../stream/fd-stream.hh"
#include "server.hh"
#include "channel.hh"

namespace mimosa
{
  namespace rpc
  {
    Server::Server()
    {
      this->onAccept(new AcceptHandler(
                       std::bind(&Server::serve, Server::Ptr(this),
                                 std::placeholders::_1)));
    }

    void
    Server::serve(Server::Ptr server, int fd)
    {
      stream::FdStream::Ptr stream = new stream::FdStream(fd);
      Channel::Ptr channel = new Channel(stream, server->service_map_);
      runtime::Fiber::start([channel]() { channel->readLoop(); });
      runtime::Fiber::start([channel]() { channel->writeLoop(); });
    }
  }
}
