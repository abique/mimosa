#include <functional>
#include <memory>

#include "server.hh"
#include "server-channel.hh"
#include "../stream/fd-stream.hh"

namespace mimosa
{
  namespace http
  {
    Server::Server()
      : read_timeout_(0),
        write_timeout_(0),
        handler_()
    {
      onAccept(new AcceptHandler(std::bind(&Server::newClient, Server::Ptr(this), std::placeholders::_1)));
    }

    void
    Server::newClient(Server::Ptr server, int fd)
    {
      stream::BufferedStream::Ptr stream(new stream::FdStream(fd));
      std::unique_ptr<ServerChannel> channel(
        new ServerChannel(stream,
                          server->handler_,
                          server->read_timeout_,
                          server->write_timeout_));
      channel->run();
    }
  }
}
