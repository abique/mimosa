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
    DispatchHandler::registerHandler(const std::string & pattern, Handler::ConstPtr handler)
    {
      handlers_[pattern] = handler;
    }

    bool
    DispatchHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      response.content_type_ = "text/html";
      for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
        if (!::fnmatch(it->first.c_str(), request.location().c_str(), FNM_PATHNAME))
          return it->second->handle(request, response);
      return error_handler_->handle(request, response, kStatusNotFound);
    }
  }
}
