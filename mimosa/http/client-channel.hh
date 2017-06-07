#pragma once

# include <sys/types.h>
# include <sys/socket.h>

# include "../non-copyable.hh"
# include "../stream/buffered-stream.hh"
# include "request-writer.hh"
# include "response-reader.hh"
# include "../string-ref.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class ClientChannel : private NonCopyable
    {
    public:
      ClientChannel();
      ~ClientChannel();

      inline void setRemoteAddr(const ::sockaddr * addr, ::socklen_t addr_len)
      {
        addr_     = addr;
        addr_len_ = addr_len;
      }

      bool connect(const std::string &host, uint16_t port, bool ssl);
      bool readResponse(Response & rp);

      inline const ::sockaddr * remoteAddr() const { return addr_; }
      inline ::socklen_t remoteAddrLen() const { return addr_len_; }

      inline void setReadTimeout(Time timeout) { read_timeout_ = timeout; }
      inline void setWriteTimeout(Time timeout) { write_timeout_ = timeout; }

      inline bool isSsl() const { return is_ssl_; }
      inline void setSsl(bool is_ssl) { is_ssl_ = is_ssl; }

      inline stream::BufferedStream::Ptr stream() const { return stream_; }

      /////////////
      // Helpers //
      /////////////

      ResponseReader::Ptr get(const std::string & url);

    private:
      friend class MessageReader<ClientChannel, Request>;
      friend class MessageWriter<ClientChannel, Response>;
      friend ResponseReader;
      friend RequestWriter;

      stream::BufferedStream::Ptr stream_;
      const ::sockaddr *          addr_;
      ::socklen_t                 addr_len_;
      Time                        read_timeout_;
      Time                        write_timeout_;
      bool                        is_ssl_;
    };
  }
}

