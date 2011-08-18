#include "../string/string-ref.hh"
#include "fs-handler.hh"
#include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    FsHandler::FsHandler(const std::string & root, int nskip)
      : root_(root),
        nskip_(nskip)
    {
    }

    bool
    FsHandler::handle(RequestReader & request, ResponseWriter & response) const
    {
      response.content_type_ = "text/html";
      response.status_ = kStatusOk;
      string::StringRef path(request.location());
      for (auto nskip = nskip_; nskip > 0; --nskip)
      {
        auto pos = path.find('/', 1);
        if (pos == string::StringRef::npos)
        {
          if (nskip == 1 && path.size() > 1)
          {
            path = string::StringRef("/");
            break;
          }
          else
            // TODO: error reporting
            return ErrorHandler::basicResponse(request, response, kStatusInternalServerError);
        }
        path = path.substr(pos);
      }
      // TODO
      return true;
    }
  }
}
