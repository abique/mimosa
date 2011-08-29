#include <functional>
#include <memory>

#include "server.hh"
#include "server-channel.hh"
#include "../stream/direct-fd-stream.hh"
#include "../stream/tls-stream.hh"

namespace mimosa
{
  namespace http
  {
    Server::Server()
      : read_timeout_(0),
        write_timeout_(0),
        handler_(),
        x509_cred_(nullptr),
        priority_cache_(nullptr),
        dh_params_(nullptr)
    {
      ::gnutls_priority_init(&priority_cache_, "NORMAL", NULL);
      ::gnutls_dh_params_init(&dh_params_);
      ::gnutls_dh_params_generate2(dh_params_, 1024);
      onAccept(new AcceptHandler(std::bind(&Server::newClient, Server::Ptr(this),
                                           std::placeholders::_1)));
    }

    Server::~Server()
    {
      ::gnutls_priority_deinit(priority_cache_);
      ::gnutls_dh_params_deinit(dh_params_);
      if (x509_cred_)
        ::gnutls_certificate_free_credentials(x509_cred_);
    }

    void
    Server::newClient(Server::Ptr server, int fd)
    {
      stream::Stream::Ptr stream(new stream::DirectFdStream(fd));
      if (server->x509_cred_)
      {
        auto tls_stream = new stream::TlsStream(stream, true);
        stream          = tls_stream;
        ::gnutls_priority_set(tls_stream->session(), server->priority_cache_);
        ::gnutls_credentials_set(tls_stream->session(), GNUTLS_CRD_CERTIFICATE, server->x509_cred_);
        int ret = ::gnutls_handshake(tls_stream->session());
        if (ret < 0)
        {
          printf("handshake failed: %s\n", gnutls_strerror(ret));
          return;
        }
        stream = tls_stream;
      }

      ServerChannel channel(new stream::BufferedStream(stream),
                            server->handler_,
                            server->read_timeout_,
                            server->write_timeout_);
      channel.run();
    }

    void
    Server::setSecure(const std::string & cert_file,
                      const std::string & key_file)
    {
      if (!x509_cred_)
        ::gnutls_certificate_allocate_credentials(&x509_cred_);
      ::gnutls_certificate_set_x509_key_file(x509_cred_, cert_file.c_str(), key_file.c_str(),
                                             GNUTLS_X509_FMT_PEM);
      ::gnutls_certificate_set_dh_params(x509_cred_, dh_params_);
    }
  }
}
