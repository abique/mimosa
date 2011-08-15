#include "fs-handler.hh"

namespace mimosa
{
  namespace http
  {
    FsHandler::FsHandler(std::string && root, int nskip)
      : root_(root),
        nskip_(nskip)
    {
    }

    bool
    FsHandler::handle(Request & request, ResponseWriter & response) const
    {
      return true;
    }
  }
}
