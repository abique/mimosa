#include "server.hh"
#include "../runtime/fiber.hh"

namespace mimosa
{
  namespace net
  {
    Server::Server()
      : fd_(0),
        stop_(false)
    {
    }

    bool
    Server::listenInet4(uint16_t port, const struct ::in_addr * interface)
    {
      return true;
    }

    void
    Server::acceptLoop()
    {
      while (!stop_)
      {
        int fd = ::melon_accept(fd_, 0, 0, 0);
        if (fd < 0)
          continue;
        if (!runtime::Fiber::start([handler_, fd]() { (*handler_)(fd); }))
          ::close(fd);
      }
    }
  }
}
