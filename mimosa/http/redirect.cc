#include "response-writer.hh"
#include "redirect.hh"

namespace mimosa
{
  namespace http
  {
    bool redirect(ResponseWriter & response, const std::string & location)
    {
      response.status_ = kStatusFound;
      response.unparsed_headers_.insert({"location", location});
      return true;
    }
  }
}
