#ifndef MIMOSA_HTTP_REQUEST_HH
# define MIMOSA_HTTP_REQUEST_HH

# include <unordered>

# include "coding.hh"

namespace mimosa
{
  namespace http
  {
    class Request
    {
    public:
      Method                                       method_;
      std::string                                  raw_location_;
      uint8_t                                      proto_major_;
      uint8_t                                      proto_minor_;
      Coding                                       accept_encoding_;
      Coding                                       transfert_encoding_;
      bool                                         keep_alive_;
      std::vector<Cookie::Ptr>                     cookies_;
      uint32_t                                     content_length_;
      std::string                                  content_type_;
      std::string                                  host_;
      uint64_t                                     range_start_;
      uint64_t                                     range_end_;
      std::string                                  referer_;
      std::unordered_map<std::string, std::string> unparsed_headers_;
      std::string                                  body_;
    };
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_HH */
