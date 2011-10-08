#ifndef MIMOSA_HTTP_SERVER_HH
# define MIMOSA_HTTP_SERVER_HH

# include <gnutls/gnutls.h>

# include "handler.hh"
# include "../non-copyable.hh"
# include "../net/server.hh"
# include "../runtime/time.hh"

namespace mimosa
{
  namespace http
  {
    class Server : public net::Server,
                   private NonCopyable
    {
    public:
      MIMOSA_DEF_PTR(Server);

      Server();
      ~Server();

      inline void setHandler(Handler::Ptr handler) { handler_ = handler; }
      void setSecure(const std::string & cert_file,
                     const std::string & key_file);

    private:
      virtual void serve(int fd) const;

      runtime::Time                      read_timeout_;
      runtime::Time                      write_timeout_;
      Handler::Ptr                       handler_;
      ::gnutls_certificate_credentials_t x509_cred_;
      ::gnutls_priority_t                priority_cache_;
      ::gnutls_dh_params_t               dh_params_;
    };
  }
}

#endif /* !MIMOSA_HTTP_SERVER_HH */
