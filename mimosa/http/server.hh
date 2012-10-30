#ifndef MIMOSA_HTTP_SERVER_HH
# define MIMOSA_HTTP_SERVER_HH

# include <gnutls/gnutls.h>

# include "handler.hh"
# include "../non-copyable.hh"
# include "../net/server.hh"
# include "../time.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class Server : public net::Server
    {
    public:
      MIMOSA_DEF_PTR(Server);

      Server();
      ~Server();

      inline void setHandler(Handler::Ptr handler) { handler_ = handler; }
      void setSecure(const std::string & cert_file,
                     const std::string & key_file);

      inline void setReadTimeout(Time timeout) { read_timeout_ = timeout; }
      inline void setWriteTimeout(Time timeout) { write_timeout_ = timeout; }

      virtual void serve(int                fd,
                         const ::sockaddr * address,
                         socklen_t          address_len) const override;

    private:

      Time                               read_timeout_;
      Time                               write_timeout_;
      Handler::Ptr                       handler_;
      ::gnutls_certificate_credentials_t x509_cred_;
      ::gnutls_priority_t                priority_cache_;
      ::gnutls_dh_params_t               dh_params_;
    };
  }
}

#endif /* !MIMOSA_HTTP_SERVER_HH */
