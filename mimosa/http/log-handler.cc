#include "log.hh"
#include "log-handler.hh"

namespace mimosa
{
  namespace http
  {
    bool
    LogHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      http_log->info("%s %s:%s",
                     methodString(request.method()), request.host(),
                     request.rawLocation());
      return handler_->handle(request, response);
    }
  }
}
