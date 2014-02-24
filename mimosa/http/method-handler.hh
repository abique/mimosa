#ifndef MIMOSA_HTTP_METHOD_HANDLER_HH
# define MIMOSA_HTTP_METHOD_HANDLER_HH

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * This handler dispatch each http method to a specific handler.
     * The default handler is "kStatusNotImplemented".
     */
    class MethodHandler : public Handler
    {
    public:
      virtual bool head(RequestReader & request,
                        ResponseWriter & response) const;

      virtual bool get(RequestReader & request,
                       ResponseWriter & response) const;

      virtual bool post(RequestReader & request,
                        ResponseWriter & response) const;

      virtual bool put(RequestReader & request,
                       ResponseWriter & response) const;

      virtual bool del(RequestReader & request,
                       ResponseWriter & response) const;

      virtual bool trace(RequestReader & request,
                         ResponseWriter & response) const;

      virtual bool options(RequestReader & request,
                           ResponseWriter & response) const;

      virtual bool connect(RequestReader & request,
                           ResponseWriter & response) const;

      virtual bool patch(RequestReader & request,
                         ResponseWriter & response) const;

      virtual bool handle(RequestReader & request,
                          ResponseWriter & response) const override final;
    };
  }
}

#endif /* !MIMOSA_HTTP_METHOD_HANDLER_HH */
