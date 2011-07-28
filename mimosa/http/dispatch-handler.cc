#include "dispatch-handler.hh"

namespace mimosa
{
  namespace http
  {
    void
    DispatchHandler::registerHandler(const std::string & pattern, Handler::ConstPtr handler)
    {
      handlers_[pattern] = handler;
    }
  }
}
