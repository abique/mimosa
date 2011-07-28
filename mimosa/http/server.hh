#ifndef MIMOSA_HTTP_SERVER_HH
# define MIMOSA_HTTP_SERVER_HH

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    class Server
    {
    public:
      Server();
      void setHandler(Handler::Ptr & handler);

    private:
      Handler::Ptr handler_;
    };
  }
}

#endif /* !MIMOSA_HTTP_SERVER_HH */
