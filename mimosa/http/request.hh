#ifndef MIMOSA_HTTP_REQUEST_HH
# define MIMOSA_HTTP_REQUEST_HH

# include <string>
# include <vector>
# include <unordered_map>

# include "../stream/stream.hh"
# include "coding.hh"
# include "method.hh"

namespace mimosa
{
  namespace http
  {
    class Request
    {
    public:
      Request();

      Method                                            method_;
      std::string                                       raw_location_;
      uint8_t                                           proto_major_;
      uint8_t                                           proto_minor_;
      int                                               accept_encoding_; // Coding bitfield
      bool                                              keep_alive_;
      std::unordered_multimap<std::string, std::string> cookies_;
      uint64_t                                          content_length_;
      std::string                                       content_type_;
      std::string                                       host_;
      uint64_t                                          range_start_;
      uint64_t                                          range_end_;
      std::string                                       referrer_;
      std::unordered_multimap<std::string, std::string> unparsed_headers_;
      std::string                                       user_agent_;
      stream::Stream::Ptr                               body_;
    };
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_HH */
