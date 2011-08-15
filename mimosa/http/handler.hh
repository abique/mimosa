#ifndef MIMOSA_HTTP_HANDLER_HH
# define MIMOSA_HTTP_HANDLER_HH

# include "../ref-countable.hh"

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
      virtual bool handle(Request & request, ResponseWriter & response) const = 0;
    };
  }
}

#endif /* !MIMOSA_HTTP_HANDLER_HH */
