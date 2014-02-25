#include "response-writer.hh"
#include "redirect.hh"

namespace mimosa
{
  namespace http
  {
    bool redirect(ResponseWriter & response, const std::string & location)
    {
      response.setStatus(kStatusFound);
      response.setLocation(location);
      return true;
    }
  }
}
