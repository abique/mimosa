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
    Server::serve(int                fd,
                  const ::sockaddr * address,
                  socklen_t          address_len) const
    {
      stream::FdStream::Ptr stream = new stream::FdStream(fd);
      Channel::Ptr channel = new Channel(stream.get(), service_map_);
      channel->start();
      channel->wait();
    }
  }
}
