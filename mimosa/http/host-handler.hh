#pragma once

# include <string>
# include <map>

# include "handler.hh"
# include "error-handler.hh"

namespace mimosa
{
  namespace http
  {
    class HostHandler : public Handler
    {
    public:
      HostHandler();

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

