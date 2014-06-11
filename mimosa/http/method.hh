#ifndef MIMOSA_HTTP_METHOD_HH
# define MIMOSA_HTTP_METHOD_HH

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
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

      // WebDAV
      kMethodPropfind,
      kMethodProppatch,
      kMethodMkcol,
      kMethodCopy,
      kMethodMove,
      kMethodLock,
      kMethodUnlock,
    };

    /**
     * @ingroup Http
     */
    const char * methodString(Method method);
  }
}

#endif /* !MIMOSA_HTTP_METHOD_HH */
