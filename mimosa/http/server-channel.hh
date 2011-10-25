#ifndef MIMOSA_HTTP_SERVERCHANNEL_HH
# define MIMOSA_HTTP_SERVERCHANNEL_HH

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

    private:
      bool readRequest();
      bool runHandler();
      bool sendResponse();

      void requestTimeout();
      void badRequest();

      inline runtime::Time readTimeout() const
      {
        return read_timeout_ > 0 ? runtime::time() + read_timeout_ : 0;
      }

      inline runtime::Time writeTimeout() const
      {
        return write_timeout_ > 0 ? runtime::time() + write_timeout_ : 0;
      }

      stream::BufferedStream::Ptr stream_;
      Handler::Ptr                handler_;
      runtime::Time               read_timeout_;
      runtime::Time               write_timeout_;
      runtime::Time               timeout_;
      uint32_t                    header_max_size_;
      RequestReader::Ptr          request_;
      ResponseWriter::Ptr         response_;
    };
  }
}

#endif // MIMOSA_HTTP_SERVERCHANNEL_HH
