#ifndef MIMOSA_HTTP_LOG_HANDLER_HH
# define MIMOSA_HTTP_LOG_HANDLER_HH

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class LogHandler : public Handler
    {
    public:
      virtual bool handle(RequestReader & request, ResponseWriter & response) const;

      inline void setHandler(Handler::ConstPtr handler) { handler_ = handler; }

    private:
      Handler::ConstPtr handler_;
    };
  }
}

#endif /* !MIMOSA_HTTP_LOG_HANDLER_HH */
