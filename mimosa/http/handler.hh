#ifndef MIMOSA_HTTP_HANDLER_HH
# define MIMOSA_HTTP_HANDLER_HH

# include "../function.hh"

namespace mimosa
{
  namespace http
  {
    class Resquest;
    class Response;

    typedef Function<void (Request &, Response &)> Handler;
  }
}

#endif /* !MIMOSA_HTTP_HANDLER_HH */
