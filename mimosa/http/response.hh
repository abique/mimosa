#ifndef MIMOSA_HTTP_RESPONSE_HH
# define MIMOSA_HTTP_RESPONSE_HH

# include <string>

# include "coding.hh"
# include "status.hh"
# include "cookie.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace http
  {
    class Response
    {
    public:
      bool hasBody() const;

      Status              status_;
      bool                keep_alive_;
      Coding              content_encoding_;
      Coding              transfer_encoding_;
      Cookie::Slist       cookies_;
      stream::Stream::Ptr body_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_HH */
