#include "method.hh"

namespace mimosa
{
  namespace http
  {
    const char * methodString(Method method)
    {
      switch (method)
      {
      case kMethodHead:    return "HEAD";
      case kMethodGet:     return "GET";
      case kMethodPost:    return "POST";
      case kMethodPut:     return "PUT";
      case kMethodDelete:  return "DELETE";
      case kMethodTrace:   return "TRACE";
      case kMethodOptions: return "OPTIONS";
      case kMethodConnect: return "CONNECT";
      case kMethodPatch:   return "PATCH";

      // WebDAV
      case kMethodPropfind:  return "PROPFIND";
      case kMethodProppatch: return "PROPPATCH";
      case kMethodMkcol:     return "MKCOL";
      case kMethodCopy:      return "COPY";
      case kMethodMove:      return "MOVE";
      case kMethodLock:      return "LOCK";
      case kMethodUnlock:    return "UNLOCK";

      // Custom
      case kMethodSymlink: return "SYMLINK";

      default:             return "(none)";
      }
    }
  }
}
