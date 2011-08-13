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
    class ResponseWriter;

    class Response
    {
    public:
      bool hasBody() const;

      /** Writes the header to the client.
       * If content_length_ is 0 then the content length is unspecified
       * and the connection will close after this response (keep_alive_
       * will be set to false).
       * Until you call sendResponseHeader, everything you write to
       * body is buffered, so when you finishes Response knows the
       * amount of data to be written and set content_length_ */
      bool sendResponseHeader();

      Status              status_;
      bool                keep_alive_;
      Coding              content_encoding_;
      Coding              transfer_encoding_;
      uint64_t            content_length_;
      Cookie::Slist       cookies_;
      stream::Stream::Ptr body_;

    private:
      friend class ResponseWriter;

      bool                is_response_header_sent_;
      bool                is_finished_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_HH */
