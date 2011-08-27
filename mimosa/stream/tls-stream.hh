#ifndef MIMOSA_STREAM_TLS_STREAM_HH
# define MIMOSA_STREAM_TLS_STREAM_HH

# include <gnutls/gnutls.h>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class TlsStream : public Stream
    {
    public:
      TlsStream(Stream::Ptr stream, bool is_server);
      ~TlsStream();

      inline gnutls_session_t session() { return session_; }

    private:
      Stream::Ptr      stream_;
      gnutls_session_t session_;
    };
  }
}

#endif /* !MIMOSA_STREAM_TLS_STREAM_HH */
