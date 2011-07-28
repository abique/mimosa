#ifndef MIMOSA_HTTP_DISPATCH_HANDLER_HH
# define MIMOSA_HTTP_DISPATCH_HANDLER_HH

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    class DispatchHandler : public Handler
    {
    public:
      void registerHandler(const std::string & pattern, Handler::ConstPtr handler);
      void handle(Request & request, Response & response) const;

    private:
      std::map<std::string /*pattern*/, Handler::ConstPtr /*handler*/> handlers_;
    };
  }
}

#endif /* !MIMOSA_HTTP_DISPATCH_HANDLER_HH */
