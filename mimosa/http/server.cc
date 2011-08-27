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
      onAccept(new AcceptHandler(std::bind(&Server::newClient, Server::Ptr(this),
                                           std::placeholders::_1)));
    }

    Server::~Server()
    {
    }

    void
    Server::newClient(Server::Ptr server, int fd)
    {
      ServerChannel channel(new stream::FdStream(fd),
                            server->handler_,
                            server->read_timeout_,
                            server->write_timeout_);
      channel.run();
    }

    void
    Server::setSecure(const std::string & cert_file,
                      const std::string & key_file)
    {
      cert_file_ = cert_file;
      key_file_  = key_file;
    }
  }
}
