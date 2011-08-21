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
      default:             return "(none)";
      }
    }
  }
}
