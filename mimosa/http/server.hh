#ifndef MIMOSA_HTTP_SERVER_HH
# define MIMOSA_HTTP_SERVER_HH

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    class Server : public net::Server,
                   public RefCountable<Server>
    {
    public:
      Server();
      inline void setHandler(Handler::Ptr handler) { handler_ = handler; }

    private:
      static void newClient(Server::Ptr server, int fd);

      Time read_timeout_;
      Time write_timeout_;
      Handler::Ptr handler_;
    };
  }
}

#endif /* !MIMOSA_HTTP_SERVER_HH */
