#include <cassert>

#include "server.hh"
#include "../runtime/fiber.hh"

namespace mimosa
{
  namespace net
  {
    Server::Server()
      : handler_(0),
        fd_(0),
        stop_(false),
        accept_loop_(0)
    {
    }

    Server::~Server()
    {
      stop();
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
        try {
          runtime::Fiber::start([handler_, fd]() { (*handler_)(fd); });
        } catch (...) {
          ::close(fd);
        }
      }
    }

    void
    Server::stop()
    {
      if (fd_ >= 0)
      {
        assert(accept_loop_);

        stop_ = true;
        ::melon_close(fd_);
        accept_loop_->join();
        delete accept_loop_;
        accept_loop_ = 0;
      }
      else
        assert(!accept_loop_);
    }
  }
}
