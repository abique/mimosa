#ifndef MIMOSA_HTTP_SERVERCHANNEL_HH
# define MIMOSA_HTTP_SERVERCHANNEL_HH

# include "../non-copyable.hh"
# include "../stream/buffered-stream.hh"
# include "handler.hh"
# include "request.hh"
# include "response.hh"

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

      void run();

    private:
      bool readRequest();
      bool setupBodyReader();
      bool setupResponseWriter();
      bool runHandler();
      bool sendResponse();

      void requestTimeout();
      void badRequest();

      stream::BufferedStream::Ptr stream_;
      Handler::Ptr                handler_;
      runtime::Time               read_timeout_;
      runtime::Time               write_timeout_;
      runtime::Time               timeout_;
      Request                     request_;
      Response                    response_;
    };
  }
}

#endif // MIMOSA_HTTP_SERVERCHANNEL_HH
