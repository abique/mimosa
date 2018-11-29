#include <fnmatch.h>

#include "dispatch-handler.hh"
#include "request.hh"

namespace mimosa
{
  namespace http
  {
    DispatchHandler::DispatchHandler()
      : handlers_(),
        error_handler_(new ErrorHandler)
    {
    }

    void
    DispatchHandler::registerHandler(const std::string & pattern, const Handler::ConstPtr& handler)
    {
      handlers_[pattern] = handler;
    }

    bool
    DispatchHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      for (const auto & handler : handlers_)
        if (!::fnmatch(handler.first.c_str(), request.location().c_str(), 0))
          return handler.second->handle(request, response);
      return error_handler_->handle(request, response, kStatusNotFound);
    }
  }
}
