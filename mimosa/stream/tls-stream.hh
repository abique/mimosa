#pragma once

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

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      virtual void close();

      inline gnutls_session_t session() { return session_; }

    private:
      static ssize_t readWrapper(TlsStream * stream, char * data, size_t nbytes);
      static ssize_t writeWrapper(TlsStream * stream, const char * data, size_t nbytes);
      static ssize_t writevWrapper(TlsStream * stream, const giovec_t * iov, int iovcnt);

      gnutls_session_t session_;
    };
  }
}

