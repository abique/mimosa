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
    }

    Server::~Server()
    {
      ::gnutls_priority_deinit(priority_cache_);
      ::gnutls_dh_params_deinit(dh_params_);
      if (x509_cred_)
        ::gnutls_certificate_free_credentials(x509_cred_);
    }

    void
    Server::serve(int                fd,
                  const ::sockaddr * address,
                  socklen_t          address_len) const
    {
      stream::Stream::Ptr stream(new stream::DirectFdStream(fd));
      if (x509_cred_)
      {
        auto tls_stream = new stream::TlsStream(stream, true);
        stream          = tls_stream;
        ::gnutls_priority_set(tls_stream->session(), priority_cache_);
        ::gnutls_credentials_set(tls_stream->session(), GNUTLS_CRD_CERTIFICATE, x509_cred_);
        int ret;
        do {
          ret = ::gnutls_handshake(tls_stream->session());
        } while (ret < 0 && !gnutls_error_is_fatal(ret));

        if (ret < 0)
        {
          printf("handshake failed: %s\n", gnutls_strerror(ret));
          return;
        }
        stream = tls_stream;
      }

      ServerChannel channel(new stream::BufferedStream(stream),
                            handler_,
                            read_timeout_,
                            write_timeout_);
      channel.setRemoteAddr(address, address_len);
      channel.run();
    }

    void
    Server::setSecure(const std::string & cert_file,
                      const std::string & key_file)
    {
      if (!priority_cache_)
        ::gnutls_priority_init(&priority_cache_, "NORMAL", NULL);

      if (!dh_params_)
      {
        ::gnutls_dh_params_init(&dh_params_);
        ::gnutls_dh_params_generate2(dh_params_, 1024);
      }

      if (!x509_cred_)
        ::gnutls_certificate_allocate_credentials(&x509_cred_);

      int ret = ::gnutls_certificate_set_x509_key_file(
        x509_cred_, cert_file.c_str(), key_file.c_str(), GNUTLS_X509_FMT_PEM);
      assert(ret == GNUTLS_E_SUCCESS);
      ::gnutls_certificate_set_dh_params(x509_cred_, dh_params_);
    }
  }
}
