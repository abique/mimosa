#include "method-handler.hh"
#include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    MethodHandler::MethodHandler()
      : allow_head(true),
        allow_get(true),
        allow_post(true),
        allow_put(true),
        allow_del(true),
        allow_trace(true),
        allow_options(true),
        allow_connect(true),
        allow_patch(true)
    {
    }

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
      char buffer[128] = "";
      char *p = buffer;

#define APPEND_METHOD(Name, Str)                  \
      do {                                        \
        if (allow_##Name) {                       \
          if (p != buffer) {                      \
            *p = ',';                             \
            ++p;                                  \
          }                                       \
          strcpy(p, Str);                         \
          p += sizeof (Str) - 1;                  \
        }                                         \
      } while (0)

      APPEND_METHOD(head, "HEAD");
      APPEND_METHOD(get, "GET");
      APPEND_METHOD(post, "POST");
      APPEND_METHOD(put, "PUT");
      APPEND_METHOD(del, "DELETE");
      APPEND_METHOD(trace, "TRACE");
      APPEND_METHOD(options, "OPTIONS");
      APPEND_METHOD(connect, "CONNECT");
      APPEND_METHOD(patch, "PATCH");

#undef APPEND_METHOD

      response.addHeader("Allow", buffer);
      return true;
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
