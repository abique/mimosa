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
    class ServerChannel : private NonCopyable
    {
    public:
      ServerChannel(stream::BufferedStream::Ptr stream,
                    Handler::Ptr                handler,
                    runtime::Time               read_timeout  = 0,
                    runtime::Time               write_timeout = 0);
      ~ServerChannel();

      void run();

      inline void setRemoteAddr(const ::sockaddr * addr, ::socklen_t addr_len)
      {
        addr_ = addr;
        addr_len_ = addr_len;
      }

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
    };
  }
}

#endif // MIMOSA_HTTP_SERVERCHANNEL_HH
