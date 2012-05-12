#ifndef MIMOSA_STREAM_TLS_STREAM_HH
# define MIMOSA_STREAM_TLS_STREAM_HH

# include <gnutls/gnutls.h>

# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class TlsStream : public Filter
    {
    public:
      MIMOSA_DEF_PTR(TlsStream);

      TlsStream(Stream::Ptr stream, bool is_server);
      ~TlsStream();

      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0);

      virtual void close();

      inline gnutls_session_t session() { return session_; }

    private:
      static ssize_t readWrapper(TlsStream * stream, char * data, size_t nbytes);
      static ssize_t writeWrapper(TlsStream * stream, const char * data, size_t nbytes);
      static ssize_t writevWrapper(TlsStream * stream, const giovec_t * iov, int iovcnt);

      gnutls_session_t session_;
      Time    read_timeout_;
      Time    write_timeout_;
    };
  }
}

#endif /* !MIMOSA_STREAM_TLS_STREAM_HH */
