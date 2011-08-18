#ifndef MIMOSA_HTTP_DISPATCH_HANDLER_HH
# define MIMOSA_HTTP_DISPATCH_HANDLER_HH

# include <string>
# include <map>

# include "handler.hh"
# include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    class DispatchHandler : public Handler
    {
    public:
      DispatchHandler();

      /**
       * @param pattern a globbing pattern */
      void registerHandler(const std::string & pattern, Handler::ConstPtr handler);
      virtual bool handle(Request & request, ResponseWriter & response) const;

      inline void setErrorHandler(ErrorHandler::ConstPtr handler) { error_handler_ = handler; }

    private:
      std::map<std::string /*pattern*/, Handler::ConstPtr /*handler*/> handlers_;
      ErrorHandler::ConstPtr                                           error_handler_;
    };
  }
}

#endif /* !MIMOSA_HTTP_DISPATCH_HANDLER_HH */
