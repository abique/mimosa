#include <fnmatch.h>

#include "host-handler.hh"

namespace mimosa
{
  namespace http
  {
    HostHandler::HostHandler()
      : handlers_(),
        error_handler_(new ErrorHandler)
    {
    }

    void
    HostHandler::registerHandler(const std::string & pattern, Handler::ConstPtr handler)
    {
      handlers_[pattern] = handler;
    }

    bool
    HostHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
        if (!::fnmatch(it->first.c_str(), request.host().c_str(), 0))
          return it->second->handle(request, response);
      return error_handler_->handle(request, response, kStatusNotFound);
    }
  }
}
