#ifndef MIMOSA_HTTP_SERVERCHANNEL_HH
# define MIMOSA_HTTP_SERVERCHANNEL_HH

# include <sys/types.h>
# include <sys/socket.h>

# include "../non-copyable.hh"
# include "../stream/buffered-stream.hh"
# include "handler.hh"
# include "request.hh"
# include "response-writer.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class ServerChannel : private NonCopyable
    {
    public:
      ServerChannel(stream::BufferedStream::Ptr stream,
                    Handler::Ptr                handler);
      ~ServerChannel();

      void run();

      inline void setRemoteAddr(const ::sockaddr * addr, ::socklen_t addr_len)
      {
        addr_ = addr;
        addr_len_ = addr_len;
      }

      inline const ::sockaddr * remoteAddr() const { return addr_; }
      inline ::socklen_t remoteAddrLen() const { return addr_len_; }

      inline void setReadTimeout(Time timeout) { read_timeout_ = timeout; }
      inline void setWriteTimeout(Time timeout) { write_timeout_ = timeout; }

      inline bool isSsl() const { return is_ssl_; }
      inline void setSsl(bool is_ssl) { is_ssl_ = is_ssl; }

    private:
      friend class RequestReader;
      friend class ResponseWriter;

      bool readRequest();
      bool runHandler();
      bool sendResponse();

      void requestTimeout();
      void badRequest();

      stream::BufferedStream::Ptr stream_;
      Handler::Ptr                handler_;
      RequestReader::Ptr          request_;
      ResponseWriter::Ptr         response_;
      const ::sockaddr *          addr_;
      ::socklen_t                 addr_len_;
      Time                        read_timeout_;
      Time                        write_timeout_;
      bool                        is_ssl_;
    };
  }
}

#endif // MIMOSA_HTTP_SERVERCHANNEL_HH
