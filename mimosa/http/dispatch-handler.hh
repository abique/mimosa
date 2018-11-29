#pragma once

# include <string>
# include <map>

# include "handler.hh"
# include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class DispatchHandler : public Handler
    {
    public:
      DispatchHandler();

      /**
       * @param pattern a globbing pattern */
      void registerHandler(const std::string & pattern, const Handler::ConstPtr& handler);
      virtual bool handle(RequestReader & request, ResponseWriter & response) const;

      inline void setErrorHandler(ErrorHandler::ConstPtr handler) { error_handler_ = handler; }

    private:
      std::map<std::string /*pattern*/, Handler::ConstPtr /*handler*/> handlers_;
      ErrorHandler::ConstPtr                                           error_handler_;
    };
  }
}

