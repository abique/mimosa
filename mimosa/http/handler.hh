#ifndef MIMOSA_HTTP_HANDLER_HH
# define MIMOSA_HTTP_HANDLER_HH

# include "../ref-countable.hh"
# include "request-reader.hh"
# include "response-writer.hh"

namespace mimosa
{
  namespace http
  {
    class Request;
    class ResponseWriter;

    class Handler : public RefCountable<Handler>
    {
    public:
      virtual ~Handler() {}
      virtual bool handle(RequestReader & request, ResponseWriter & response) const = 0;
    };
  }
}

#endif /* !MIMOSA_HTTP_HANDLER_HH */
