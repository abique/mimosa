#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

#include "../function.hh"
#include "../thread.hh"
#include "../non-copyable.hh"
#include "../non-movable.hh"

namespace mimosa
{
  namespace net
  {
    class Server : public RefCountable<Server>, private NonCopyable, private NonMovable
    {
    public:
      constexpr Server() = default;
      ~Server() noexcept;

      void listenUnix(const std::string & path);
      void listenInet4(uint16_t port, const struct ::in_addr * interface = nullptr);
      void listenInet6(uint16_t port, const struct ::in6_addr * interface = nullptr);

      inline int fd() const { return fd_; }
      int accept(::sockaddr *  address     = nullptr,
                 ::socklen_t * address_len = nullptr,
                 Time          timeout     = 0) const;

      /// @param new_thread if true, after accept return a valid fd, serveOne
      /// will call serve(fd) in a new thread
      void serveOne(Time accept_timeout = 0, bool new_thread = true) const;

      void close();

      /// The default implementation just closes the fd.
      virtual void serve(int                fd,
                         const ::sockaddr * address,
                         socklen_t          address_len) const;

    private:
      int  fd_ = -1;
    };
  }
}

