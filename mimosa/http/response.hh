#ifndef MIMOSA_HTTP_RESPONSE_HH
# define MIMOSA_HTTP_RESPONSE_HH

# include <string>
# include <unordered_map>

# include "coding.hh"
# include "status.hh"
# include "cookie.hh"
# include "../kvs.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class Response
    {
    public:
      Response();

      /** convert the response to an http response header */
      bool print(stream::Stream & stream, Time timeout) const;
      std::string toHttpHeader() const;

      void clear();

      Status         status_;
      bool           keep_alive_;
      Coding         content_encoding_;
      Coding         transfer_encoding_;
      int64_t        content_length_;     ///< -1: auto
      std::string    content_type_;
      Cookie::Slist  cookies_;
      kvs unparsed_headers_;
      time_t         last_modified_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_HH */
