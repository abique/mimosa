#include "log.hh"
#include "log-handler.hh"

namespace mimosa
{
  namespace http
  {
    bool
    LogHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      MIMOSA_LOG(Info, http_log, "%s %s:%s",
                 methodString(request.method()), request.host(),
                 request.rawLocation());
      return handler_->handle(request, response);
    }
  }
}
