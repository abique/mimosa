#include <sys/socket.h>
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
      stop();
      fd_ = ::socket(AF_INET, SOCK_STREAM, PF_INET);
      if (fd_ < 0)
        return false;

      // enable reuse of the port after the close
      static const int enable = 1;
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enalbe, sizeof(enable));

      struct sockaddr_in addr;
      addr.sin_addr.s_addr = interface ? interface->s_addr : INADDR_ANY;
      addr.sin_family = AF_INET;
      addr.sin_port = port;
      if (::bind(fd_, (struct sockaddr *)&addr, sizeof (addr)))
        goto failure;
      if (listen(fd_, 10))
        goto failure;
      try {
        this->accept_loop_ = new runtime::Fiber(std::bind(&Server::acceptLoop, this));
      } catch (...) {
        goto failure;
      }
      return true;

      failure:
      ::close(fd_);
      fd_ = -1;
      this->accept_loop_ = 0;
      return false;
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
        fd_ = -1;
      }
      else
        assert(!accept_loop_);
    }
  }
}
