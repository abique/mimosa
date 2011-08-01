#include <functional>

#include "server.hh"
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
      std::scoped_ptr<ServerChannel> channel =
        new ServerChannel(stream, handler, read_timeout_, write_timeout_);
      channel->run();
    }
  }
}
