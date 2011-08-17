#include "server-channel.hh"

namespace mimosa
{
  namespace http
  {
    ServerChannel::ServerChannel(stream::FdStream::Ptr stream,
                                 Handler::Ptr          handler,
                                 runtime::Time         read_timeout,
                                 runtime::Time         write_timeout)
      : stream_(stream),
        handler_(handler),
        read_timeout_(read_timeout),
        write_timeout_(write_timeout),
        timeout_(0),
        request_(),
        response_(nullptr)
    {
    }

    void
    ServerChannel::run()
    {
      do {
        if (!readRequest() ||
            !setupBodyReader() ||
            !setupResponseWriter() ||
            !runHandler() ||
            !sendResponse())
          break;
      } while (response_->keep_alive_);
    }

    bool
    ServerChannel::readRequest()
    {
      stream::Buffer::Ptr buffer = stream_->readUntil("\r\n\r\n", readTimeout());
      if (!buffer)
      {
        requestTimeout();
        return false;
      }

      request_.clear();
      if (!request_.parse(buffer->data(), buffer->size() + 2))
      {
        badRequest();
        return false;
      }

      return true;
    }

    bool
    ServerChannel::setupBodyReader()
    {
      //assert(false && "TODO");
      return true;
    }

    bool
    ServerChannel::setupResponseWriter()
    {
      response_ = new ResponseWriter(stream_);
      return true;
    }

    bool
    ServerChannel::runHandler()
    {
      return handler_->handle(request_, *response_);
    }

    bool
    ServerChannel::sendResponse()
    {
      return response_->finish(writeTimeout());
    }

    void
    ServerChannel::requestTimeout()
    {
      assert(false);
    }

    void
    ServerChannel::badRequest()
    {
      assert(false);
    };
  }
}
