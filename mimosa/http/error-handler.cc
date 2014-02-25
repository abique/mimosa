#include <sstream>

#include "error-handler.hh"
#include "../format/format-stream.hh"

namespace mimosa
{
  namespace http
  {
    bool
    ErrorHandler::basicResponse(Request & /*request*/, ResponseWriter & response, Status status)
    {
      response.setStatus(status);
      format::format(
        response,
        "<html><head><title>Error %v - %v</title>"
        "</head><body>Error %v - %v</body></html>",
        status, statusToString(status),
        status, statusToString(status));
      return true;
    }
  }
}
