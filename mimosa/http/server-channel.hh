#ifndef MIMOSA_HTTP_SERVERCHANNEL_HH
# define MIMOSA_HTTP_SERVERCHANNEL_HH

# include "../non-copyable.hh"
# include "../stream/buffered-stream.hh"
# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    class ServerChannel : private NonCopyable
    {
    public:
      ServerChannel(BufferedStream::Ptr stream,
                    Handler::Ptr        handler,
                    runtime::Time       read_timeout  = 0,
                    runtime::Time       write_timeout = 0);

      void run();

    private:
      bool readRequest();
      bool readBody();
      void runHandler();
      bool sendResponse();

      BufferedStream::Ptr stream_;
      Handler::Ptr        handler_;
      runtime::Time       read_timeout_;
      runtime::Time       write_timeout_;
      runtime::Time       timeout_;
      Request             request_;
      Response            response_;
    };
  }
}

#endif // MIMOSA_HTTP_SERVERCHANNEL_HH
