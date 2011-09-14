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

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);

      virtual void close();
      virtual void cancel();

      inline gnutls_session_t session() { return session_; }

    private:
      static ssize_t readWrapper(TlsStream * stream, char * data, size_t nbytes);
      static ssize_t writeWrapper(TlsStream * stream, const char * data, size_t nbytes);

      Stream::Ptr      stream_;
      gnutls_session_t session_;
      runtime::Time    read_timeout_;
      runtime::Time    write_timeout_;
    };
  }
}

#endif /* !MIMOSA_STREAM_TLS_STREAM_HH */
