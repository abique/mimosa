#ifndef MIMOSA_HTTP_HANDLER_HH
# define MIMOSA_HTTP_HANDLER_HH

# include "../ref-countable.hh"

namespace mimosa
{
  namespace http
  {
    class Request;
    class Response;

    class Handler : public RefCountable<Handler>
    {
    public:
      virtual ~Handler() {}
      virtual void handle(Request & request, Response & response) const = 0;
    };
  }
}

#endif /* !MIMOSA_HTTP_HANDLER_HH */
