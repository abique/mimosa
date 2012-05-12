#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cassert>
#include <cstring>
#include <memory>

#include "server.hh"
#include "accept.hh"
#include "../thread.hh"

namespace mimosa
{
  namespace net
  {
    Server::Server()
      : fd_(-1)
    {
    }

    Server::~Server()
    {
      if (fd_ >= 0)
        ::close(fd_);
    }

#define LISTEN_COMMON_1(Domain)                         \
    do {                                                \
      if (fd_ >= 0)                                     \
        ::close(fd_);                                   \
                                                        \
      fd_ = ::socket(Domain, SOCK_STREAM, 0);           \
      if (fd_ < 0)                                      \
        return false;                                   \
                                                        \
      static const int enable = 1;                      \
      ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,       \
                   &enable, sizeof(enable));            \
    } while (0)

#define LISTEN_COMMON_2()                                       \
    do {                                                        \
      if (::bind(fd_, (struct sockaddr *)&addr, sizeof (addr))) \
        goto failure;                                           \
      if (::listen(fd_, 10))                                    \
        goto failure;                                           \
      return true;                                              \
                                                                \
      failure:                                                  \
      ::close(fd_);                                             \
      fd_ = -1;                                                 \
      return false;                                             \
    } while (0)


    bool
    Server::listenInet4(uint16_t port, const struct ::in_addr * interface)
    {
      LISTEN_COMMON_1(AF_INET);

      struct sockaddr_in addr;
      ::memset(&addr, 0, sizeof (addr));
      addr.sin_addr.s_addr = interface ? interface->s_addr : INADDR_ANY;
      addr.sin_family      = AF_INET;
      addr.sin_port        = htons(port);

      LISTEN_COMMON_2();
    }

    bool
    Server::listenInet6(uint16_t port, const::in6_addr *interface)
    {
      LISTEN_COMMON_1(AF_INET6);

      // accepts ipv4 connections
      static const int disable = 0;
      ::setsockopt(fd_, IPPROTO_IPV6, IPV6_V6ONLY, &disable, sizeof (disable));

      struct sockaddr_in6 addr;
      ::memset(&addr, 0, sizeof (addr));
      addr.sin6_addr   = interface ? *interface : ::in6addr_any;
      addr.sin6_family = AF_INET6;
      addr.sin6_port   = htons(port);

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

    int
    Server::accept(::sockaddr *  address,
                   ::socklen_t * address_len,
                   Time timeout) const
    {
      return net::accept(fd_, address, address_len, timeout);
    }

    void
    Server::serveOne(Time accept_timeout, bool new_thread) const
    {
      union {
        ::sockaddr addr;
        ::sockaddr_in in_addr;
        ::sockaddr_in6 in6_addr;
        ::sockaddr_un un_addr;
      } addr;
      ::socklen_t addr_len = sizeof (addr);

      int fd = accept(&addr.addr, &addr_len, accept_timeout);
      if (fd >= 0)
      {
        Server::ConstPtr server(this);
        if (new_thread)
          Thread([server, fd, addr, addr_len] {
              try {
                server->serve(fd, &addr.addr, addr_len);
              } catch (...) {
              }
            }).start();
        else
          serve(fd, &addr.addr, addr_len);
      }
    }
  }
}
