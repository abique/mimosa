#include <fnmatch.h>

#include "host-handler.hh"

namespace mimosa
{
  namespace http
  {
    HostHandler::HostHandler()
      : error_handler_(new ErrorHandler)
    {
    }

    void
    HostHandler::registerHandler(const std::string & pattern, const Handler::ConstPtr& handler)
    {
      handlers_[pattern] = handler;
    }

    bool
    HostHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      for (const auto & handler : handlers_)
        if (!::fnmatch(handler.first.c_str(), request.host().c_str(), 0))
          return handler.second->handle(request, response);
      return error_handler_->handle(request, response, kStatusNotFound);
    }
  }
}
