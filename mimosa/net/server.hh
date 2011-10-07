#ifndef MIMOSA_NET_SERVER_HH
# define MIMOSA_NET_SERVER_HH

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include <string>

# include "../function.hh"
# include "../runtime/thread.hh"

namespace mimosa
{
  namespace net
  {
    class Server : public RefCountable<Server>
    {
    public:
      Server();
      ~Server();

      typedef Function<void (int fd)> AcceptHandler;

      bool listenUnix(const std::string & path);
      bool listenInet4(uint16_t port, const struct ::in_addr * interface = 0);
      bool listenInet6(uint16_t port, const struct ::in6_addr * interface = 0);
      inline void onAccept(AcceptHandler::Ptr handler) { handler_ = handler; }
      void stop();

      static void setupFd(int fd);

    private:
      static void acceptLoop(Server::Ptr server);

      AcceptHandler::Ptr handler_;
      int                fd_;
      bool               stop_;
      runtime::Fiber *   accept_loop_;
    };
  }
}

#endif /* !MIMOSA_NET_SERVER_HH */
