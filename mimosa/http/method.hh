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
      kMethodPost,
      kMethodPut,
      kMethodDelete,
      kMethodTrace,
      kMethodOptions,
      kMethodConnect,
      kMethodPatch,
    };

    const char * methodString(Method method);
  }
}

#endif /* !MIMOSA_HTTP_METHOD_HH */
