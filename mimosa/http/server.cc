#include "server.hh"
#include "../bind.hh"

namespace mimosa
{
  namespace http
  {
    Server::Server()
      : read_timeout_(0),
        write_timeout_(0),
        handler_()
    {
      onAccept(bind(std::bind(&Server::newClient, this, _1)));
    }

    void
    Server::setHandler(Handler::Ptr handler)
    {
      handler_ = handler;
    }

    void
    Server::newClient(Server::Ptr server, int fd)
    {
      stream::Stream::Ptr stream(new stream::FdStream(fd));
    }
  }
}
