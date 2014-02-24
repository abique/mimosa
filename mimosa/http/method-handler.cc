#include "method-handler.hh"
#include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    bool
    MethodHandler::head(RequestReader & request,
                        ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::get(RequestReader & request,
                       ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::post(RequestReader & request,
                        ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::put(RequestReader & request,
                       ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::del(RequestReader & request,
                       ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::trace(RequestReader & request,
                         ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::options(RequestReader & request,
                           ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::connect(RequestReader & request,
                           ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::patch(RequestReader & request,
                         ResponseWriter & response) const
    {
      return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
    }

    bool
    MethodHandler::handle(RequestReader & request,
                          ResponseWriter & response) const
    {
      switch (request.method()) {
      case kMethodPost:
        return post(request, response);

      case kMethodGet:
        return get(request, response);

      case kMethodPut:
        return put(request, response);

      case kMethodDelete:
        return del(request, response);

      case kMethodHead:
        return head(request, response);

      case kMethodTrace:
        return trace(request, response);

      case kMethodOptions:
        return options(request, response);

      case kMethodConnect:
        return connect(request, response);

      case kMethodPatch:
        return patch(request, response);

      default:
        return ErrorHandler::basicResponse(request, response, kStatusNotImplemented);
      }
    }
  }
}
