#pragma once

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

      // Custom??
      kMethodMimosaSymlink,
    };

    /**
     * @ingroup Http
     */
    const char * methodString(Method method);
  }
}

