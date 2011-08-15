#ifndef MIMOSA_HTTP_RESPONSE_HH
# define MIMOSA_HTTP_RESPONSE_HH

# include <string>
# include <unordered_map>

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
      stream::Stream::Ptr body() const;

      /** Writes the header to the client.
       * If content_length_ is 0 then the content length is unspecified
       * and the connection will close after this response (keep_alive_
       * will be set to false).
       * Until you call sendResponseHeader, everything you write to
       * body is buffered, so when you finishes Response knows the
       * amount of data to be written and set content_length_ */
      bool sendResponseHeader();

      /** convert the response to an http response header */
      std::string && toHttpHeader() const;

      Status              status_;
      bool                keep_alive_;
      Coding              content_encoding_;
      Coding              transfer_encoding_;
      uint64_t            content_length_;
      Cookie::Slist       cookies_;
      std::unordered_map<std::string, std::string> unparsed_headers_;

    private:
      friend class ResponseWriter;

      bool                is_response_header_sent_;
      bool                is_finished_;
      stream::Stream::Ptr body_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_HH */
