#include <sstream>

#include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    bool
    ErrorHandler::basicResponse(Request & request, ResponseWriter & response, Status status)
    {
      std::ostringstream os;
      os << "<html><head><title>Error " << status << " - "
         << statusToString(status) << "</title></head><body>Error "
         << status << " - " << statusToString(status) << "</body></html>";
      response.status_ = status;
      auto str = os.str();
      response.write(str.data(), str.size());
      return true;
    }
  }
}
