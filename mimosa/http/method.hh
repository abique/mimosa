#ifndef MIMOSA_HTTP_METHOD_HH
# define MIMOSA_HTTP_METHOD_HH

namespace mimosa
{
  namespace http
  {
    enum Method
    {
      kMethodHead,
      kMethodGet,
      kMethodPut,
      kMethodDelete,
      kMethodTrace,
      kMethodOptions,
      kMethodConnect,
      kMethodPatch,
    };
  }
}

#endif /* !MIMOSA_HTTP_METHOD_HH */
