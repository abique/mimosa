#ifndef MIMOSA_HTTP_RESPONSE_HH
# define MIMOSA_HTTP_RESPONSE_HH

# include <string>

# include "coding.hh"
# include "status.hh"
# include "../stream/steram.hh"

namespace mimosa
{
  namespace http
  {
    class Response
    {
    public:
      Status                   status_;
      bool                     keep_alive_;
      Coding                   content_encoding_;
      Coding                   transfer_encoding_;
      std::vector<Cookie::Ptr> cookies_;
      stream::Stream::Ptr      body_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_HH */
