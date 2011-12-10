#ifndef MIMOSA_NET_SERVER_HH
# define MIMOSA_NET_SERVER_HH

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include <string>

# include "../function.hh"
# include "../runtime/thread.hh"
# include "../non-copyable.hh"

namespace mimosa
{
  namespace net
  {
    class Server : public RefCountable<Server>,
                   private NonCopyable
    {
    public:
      Server();
      ~Server();

      bool listenUnix(const std::string & path);
      bool listenInet4(uint16_t port, const struct ::in_addr * interface = 0);
      bool listenInet6(uint16_t port, const struct ::in6_addr * interface = 0);

      inline int fd() const { return fd_; }
      int accept(runtime::Time timeout = 0) const;

      /// @param new_thread if true, after accept return a valid fd, serveOne
      /// will call serve(fd) in a new thread
      void serveOne(runtime::Time accept_timeout = 0, bool new_thread = true) const;

    protected:
      virtual void serve(int fd) const = 0;

    private:
      int  fd_;
    };
  }
}

#endif /* !MIMOSA_NET_SERVER_HH */
