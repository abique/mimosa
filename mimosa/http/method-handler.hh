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
      MethodHandler();

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

    protected:
      unsigned allow_head : 1;
      unsigned allow_get : 1;
      unsigned allow_post : 1;
      unsigned allow_put : 1;
      unsigned allow_del : 1;
      unsigned allow_trace : 1;
      unsigned allow_options : 1;
      unsigned allow_connect : 1;
      unsigned allow_patch : 1;
    };
  }
}

#endif /* !MIMOSA_HTTP_METHOD_HANDLER_HH */
