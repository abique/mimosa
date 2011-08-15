#ifndef MIMOSA_HTTP_ERROR_HANDLER_HH
# define MIMOSA_HTTP_ERROR_HANDLER_HH

# include "handler.hh"
# include "status.hh"

namespace mimosa
{
  namespace http
  {
    class ErrorHandler : public RefCountable<ErrorHandler>
    {
    public:
      inline virtual ~ErrorHandler() {}

      inline virtual bool handle(Request & request, ResponseWriter & response, Status status) const
      {
        return basicResponse(request, response, status);
      }

      static bool basicResponse(Request & request, ResponseWriter & response, Status status);
    };
  }
}

#endif /* !MIMOSA_HTTP_ERROR_HANDLER_HH */
