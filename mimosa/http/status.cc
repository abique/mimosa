#include "status.hh"

namespace mimosa
{
  namespace http
  {
    const char * statusToString(int status)
    {
      switch (status)
      {
      case kStatusContinue:                     return "Continue";
      case kStatusSwitchingProtocols:           return "SwitchingProtocols";

      case kStatusOk:                           return "OK";
      case kStatusCreated:                      return "Created";
      case kStatusAccepted:                     return "Accepted";
      case kStatusNonAuthoritativeInfo:         return "NonAuthoritativeInfo";
      case kStatusNoContent:                    return "NoContent";
      case kStatusResetContent:                 return "ResetContent";
      case kStatusPartialContent:               return "PartialContent";

      case kStatusMultipleChoices:              return "MultipleChoices";
      case kStatusMovedPermanently:             return "MovedPermanently";
      case kStatusFound:                        return "Found";
      case kStatusSeeOther:                     return "SeeOther";
      case kStatusNotModified:                  return "NotModified";
      case kStatusUseProxy:                     return "UseProxy";
      case kStatusTemporaryRedirect:            return "TemporaryRedirect";

      case kStatusBadRequest:                   return "BadRequest";
      case kStatusUnauthorized:                 return "Unauthorized";
      case kStatusPaymentRequired:              return "PaymentRequired";
      case kStatusForbidden:                    return "Forbidden";
      case kStatusNotFound:                     return "NotFound";
      case kStatusMethodNotAllowed:             return "MethodNotAllowed";
      case kStatusNotAcceptable:                return "NotAcceptable";
      case kStatusProxyAuthRequired:            return "ProxyAuthRequired";
      case kStatusRequestTimeout:               return "RequestTimeout";
      case kStatusConflict:                     return "Conflict";
      case kStatusGone:                         return "Gone";
      case kStatusLengthRequired:               return "LengthRequired";
      case kStatusPreconditionFailed:           return "PreconditionFailed";
      case kStatusRequestEntityTooLarge:        return "RequestEntityTooLarge";
      case kStatusRequestURITooLong:            return "RequestURITooLong";
      case kStatusUnsupportedMediaType:         return "UnsupportedMediaType";
      case kStatusRequestedRangeNotSatisfiable: return "RequestedRangeNotSatisfiable";
      case kStatusExpectationFailed:            return "ExpectationFailed";

      case kStatusInternalServerError:          return "InternalServerError";
      case kStatusNotImplemented:               return "NotImplemented";
      case kStatusBadGateway:                   return "BadGateway";
      case kStatusServiceUnavailable:           return "ServiceUnavailable";
      case kStatusGatewayTimeout:               return "GatewayTimeout";
      case kStatusHTTPVersionNotSupported:      return "HTTPVersionNotSupported";
      case kStatusInsufficientStorage:          return "InsufficientStorage";

      default: return "(unknown status)";
      }
    }
  }
}
