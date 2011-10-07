#include <sys/socket.h>
#include <sys/un.h>
#include <cassert>
#include <cstring>
#include <memory>

#include "server.hh"
#include "accept.hh"
#include "../runtime/thread.hh"

namespace mimosa
{
  namespace net
  {
    Server::Server()
      : handler_(0),
        fd_(-1),
        stop_(false),
        accept_loop_(0)
    {
    }

    Server::~Server()
    {
      stop();
    }

    void
    Server::setupFd(int fd)
    {
      // enable reuse of the port after the close
      static const int enable = 1;
      ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    }

#define LISTEN_COMMON_1(Domain)                 \
    do {                                        \
      stop();                                   \
      fd_ = ::socket(AF_INET, SOCK_STREAM, 0);  \
      if (fd_ < 0)                              \
        return false;                           \
      setupFd(fd_);                             \
    } while (0)

#define LISTEN_COMMON_2()                                               \
    do {                                                                \
      if (::bind(fd_, (struct sockaddr *)&addr, sizeof (addr)))         \
        goto failure;                                                   \
      if (::listen(fd_, 10))                                            \
        goto failure;                                                   \
      try {                                                             \
        this->accept_loop_ = new runtime::Fiber(                        \
          std::bind(&Server::acceptLoop, Server::Ptr(this)));           \
      } catch (...) {                                                   \
        goto failure;                                                   \
      }                                                                 \
      return true;                                                      \
                                                                        \
      failure:                                                          \
      ::close(fd_);                                                     \
      fd_ = -1;                                                         \
      this->accept_loop_ = 0;                                           \
      return false;                                                     \
    } while (0)


    bool
    Server::listenInet4(uint16_t port, const struct ::in_addr * interface)
    {
      LISTEN_COMMON_1(AF_INET);

      struct sockaddr_in addr;
      ::memset(&addr, 0, sizeof (addr));
      addr.sin_addr.s_addr = interface ? interface->s_addr : INADDR_ANY;
      addr.sin_family = AF_INET;
      addr.sin_port = ::htons(port);

      LISTEN_COMMON_2();
    }

    bool
    Server::listenInet6(uint16_t port, const::in6_addr *interface)
    {
      LISTEN_COMMON_1(AF_INET6);

      struct sockaddr_in6 addr;
      ::memset(&addr, 0, sizeof (addr));
      addr.sin6_addr = interface ? *interface : ::in6addr_any;
      addr.sin6_family = AF_INET6;
      addr.sin6_port = ::htons(port);

      LISTEN_COMMON_2();
    }

    bool
    Server::listenUnix(const std::string &path)
    {
      LISTEN_COMMON_1(AF_UNIX);

      std::unique_ptr<char> data(new char[
          ((size_t) (((::sockaddr_un *) 0)->sun_path)) + path.size() + 1]);
      ::sockaddr_un & addr = *(::sockaddr_un*)data.get();
      addr.sun_family = AF_UNIX;
      ::memcpy(addr.sun_path, path.c_str(), path.size() + 1);
      ::unlink(path.c_str());

      LISTEN_COMMON_2();
    }

    void
    Server::acceptLoop(Server::Ptr server)
    {
      while (!server->stop_)
      {
        int fd = net::accept(server->fd_, 0, 0, 0);
        if (fd < 0)
          continue;
        try {
          runtime::Fiber::start([server, fd]() { (*server->handler_)(fd); });
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
        bool joined = false;
        do {
          puts("canceling io...");
          ::pthread_cancel(accept_loop_->threadId());
          joined = accept_loop_->timedJoin(runtime::time() + runtime::millisecond);
        } while (!joined);
        delete accept_loop_;
        ::close(fd_);
        accept_loop_ = 0;
        fd_ = -1;
      }
      else
        assert(!accept_loop_);
    }
  }
}
