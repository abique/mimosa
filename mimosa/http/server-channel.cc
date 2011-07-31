#include "server-channel.hh"

namespace mimosa
{
  namespace http
  {
    ServerChannel::ServerChannel(BufferedStream::Ptr stream,
                                 Handler::Ptr        handler,
                                 runtime::Time       read_timeout,
                                 runtime::Time       write_timeout)
      : stream_(stream),
        handler_(handler),
        read_timeout_(read_timeout),
        write_timeout_(write_timeout)
    {
    }

    void
    ServerChannel::run()
    {
      do {
        if (!readRequest())
          break;
        if (hasBody() && !readBody())
          break;
        runHandler();
        if (!sendResponse())
          break;
      } while (keep_alive_);
    }

    bool
    ServerChannel::readRequest()
    {
      timeout_ = read_timeout_ > 0 ? runtime::time() + read_timeout_ : 0;
      std::string raw_request = stream_->readUntil("\r\n\r\n");
      return true;
    }
  }
}
