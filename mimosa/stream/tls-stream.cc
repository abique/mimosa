#include "tls-stream.hh"

namespace mimosa
{
  namespace stream
  {
    static ssize_t readWrapper(TlsStream * stream,
                               char * data,
                               size_t nbytes)
    {
      return stream->read(data, nbytes);
    }

    static ssize_t writeWrapper(TlsStream * stream,
                                const char * data,
                                size_t nbytes)
    {
      return stream->write(data, nbytes);
    }

    TlsStream::TlsStream(Stream::Ptr stream, bool is_server)
        : stream_(stream),
          session_()
    {
      gnutls_init(&session_, is_server ? GNUTLS_SERVER : GNUTLS_CLIENT);
      gnutls_transport_set_ptr(session_, this);
      gnutls_transport_set_pull_function(session_, (gnutls_pull_func)readWrapper);
      gnutls_transport_set_push_function(session_, (gnutls_push_func)writeWrapper);
      // TODO: set vec push
    }

    TlsStream::~TlsStream()
    {
      gnutls_deinit(session_);
      session_ = nullptr;
    }
  }
}
