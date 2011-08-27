#ifndef MIMOSA_HTTP_SERVER_HH
# define MIMOSA_HTTP_SERVER_HH

# include "handler.hh"
# include "../net/server.hh"
# include "../runtime/time.hh"

namespace mimosa
{
  namespace http
  {
    class Server : public net::Server
    {
    public:
      MIMOSA_DEF_PTR(Server);

      Server();
      ~Server();

      inline void setHandler(Handler::Ptr handler) { handler_ = handler; }
      void setSecure(const std::string & cert_file,
                     const std::string & key_file);

    private:
      static void newClient(Server::Ptr server, int fd);

      runtime::Time read_timeout_;
      runtime::Time write_timeout_;
      Handler::Ptr  handler_;
      std::string   cert_file_;
      std::string   key_file_;
    };
  }
}

#endif /* !MIMOSA_HTTP_SERVER_HH */
