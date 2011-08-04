#include <fnmatch.h>

#include "dispatch-handler.hh"
#include "request.hh"

namespace mimosa
{
  namespace http
  {
    void
    DispatchHandler::registerHandler(const std::string & pattern, Handler::ConstPtr handler)
    {
      handlers_[pattern] = handler;
    }

    bool
    DispatchHandler::handle(Request & request, Response & response) const
    {
      for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
        if (!::fnmatch(it->first.c_str(), request.location_.c_str(), FNM_PATHNAME))
          return it->second->handle(request, response);
      return error_handler_->handle(request, response, kStatusNotFound);
    }
  }
}
