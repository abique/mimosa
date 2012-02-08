#include "../net/print.hh"
#include "log.hh"
#include "log-handler.hh"
#include "server-channel.hh"

namespace mimosa
{
  namespace http
  {
    bool
    LogHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      http_log->info("%s %s %s:%s",
                     net::print(request.channel().remoteAddr(),
                                request.channel().remoteAddrLen()),
                     methodString(request.method()), request.host(),
                     request.rawLocation());
      return handler_->handle(request, response);
    }
  }
}
