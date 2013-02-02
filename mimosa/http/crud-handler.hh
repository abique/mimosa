#ifndef MIMOSA_HTTP_CRUD_HANDLER_HH
# define MIMOSA_HTTP_CRUD_HANDLER_HH

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * This handler helps you to map CRUD web services.
     * You have to override methods you wish to implement,
     * and the other ones will reply 501 (NotImplemented).
     */
    class CrudHandler : public Handler
    {
    public:
      /**
       * This method matches the INSERT SQL statement or the POST
       * HTTP request.
       */
      virtual bool create(RequestReader & request,
                          ResponseWriter & response) const;

      /**
       * This method matches the SELECT SQL statement or the GET
       * HTTP request.
       */
      virtual bool read(RequestReader & request,
                        ResponseWriter & response) const;

      /**
       * This method matches the UPDATE SQL statement or the PUT
       * HTTP request.
       */
      virtual bool update(RequestReader & request,
                          ResponseWriter & response) const;

      /**
       * This method matches the DELETE SQL statement or the DELETE
       * HTTP request. This method is not named delete because of the
       * C++ keyword delete.
       */
      virtual bool destroy(RequestReader & request,
                           ResponseWriter & response) const;

      /**
       * This method should not be overridden and dispatches requests,
       * over create/read/update/destroy.
       */
      virtual bool handle(RequestReader & request,
                          ResponseWriter & response) const override final;
    };
  }
}

#endif /* !MIMOSA_HTTP_CRUD_HANDLER_HH */
