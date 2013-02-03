#include "crud-handler.hh"
#include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    bool
    CrudHandler::create(RequestReader & request,
                        ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    CrudHandler::read(RequestReader & request,
                      ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    CrudHandler::update(RequestReader & request,
                        ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    CrudHandler::destroy(RequestReader & request,
                         ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    CrudHandler::handle(RequestReader & request,
                        ResponseWriter & response) const
    {
      switch (request.method()) {
      case kMethodPost:
        return create(request, response);

      case kMethodGet:
        return read(request, response);

      case kMethodPut:
        return update(request, response);

      case kMethodDelete:
        return destroy(request, response);

      case kMethodHead:
      case kMethodTrace:
      case kMethodOptions:
      case kMethodConnect:
      case kMethodPatch:
      default:
        return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
      }
    }
  }
}
