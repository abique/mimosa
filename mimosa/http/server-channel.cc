#include "error-handler.hh"
#include "server-channel.hh"

namespace mimosa
{
  namespace http
  {
    ServerChannel::ServerChannel(stream::BufferedStream::Ptr stream,
                                 Handler::Ptr                handler)
      : stream_(stream),
        handler_(handler),
        request_(new RequestReader(*this)),
        response_(new ResponseWriter(*this)),
        addr_(nullptr),
        addr_len_(0),
        read_timeout_(0),
        write_timeout_(0)
    {
    }

    ServerChannel::~ServerChannel()
    {
    }

    void
    ServerChannel::run()
    {
      do {
        request_->clear();
        response_->clear();
        if (!readRequest() ||
            !runHandler() ||
            !sendResponse())
          break;
      } while (response_->keep_alive_);
    }

    bool
    ServerChannel::readRequest()
    {
      bool found = false;
      stream_->setReadTimeout(read_timeout_ > 0 ? read_timeout_ + monotonicTimeCoarse() : 0);
      stream::Buffer::Ptr buffer = stream_->readUntil(
        "\r\n\r\n", 5 * 1024, &found);
      if (!buffer)
      {
        requestTimeout();
        return false;
      }

      if (!found)
      {
        ErrorHandler::basicResponse(*request_, *response_, kStatusRequestEntityTooLarge);
        return false;
      }

      if (!request_->parse(buffer->data(), buffer->size() + 2))
      {
        badRequest();
        return false;
      }

      response_->keep_alive_ = request_->keep_alive_;
      return request_->prepare();
    }

    bool
    ServerChannel::runHandler()
    {
      stream_->setWriteTimeout(write_timeout_ > 0 ? write_timeout_ + monotonicTimeCoarse() : 0);
      return handler_->handle(*request_, *response_);
    }

    bool
    ServerChannel::sendResponse()
    {
      if (!response_->finish())
        return false;

      // don't flush yet if we can handle next request and send just one tcp packet
      if (stream_->readyRead() == 0)
        return stream_->flush();

      return true;
    }

    void
    ServerChannel::requestTimeout()
    {
      ErrorHandler::basicResponse(*request_, *response_, kStatusRequestTimeout);
    }

    void
    ServerChannel::badRequest()
    {
      ErrorHandler::basicResponse(*request_, *response_, kStatusBadRequest);
    };
  }
}
