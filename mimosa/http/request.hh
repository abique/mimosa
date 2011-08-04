#ifndef MIMOSA_HTTP_REQUEST_HH
# define MIMOSA_HTTP_REQUEST_HH

# include <string>
# include <vector>
# include <map>
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

      void reset();

      /**
       * @brief parses the buffer
       * @return true on success and false otherwise
       */
      bool parse(const char * data, size_t size);

      /**
       * @brief parses the buffer
       * @param data the buffer is directly passed to flex so it can be
       * realloced/freed, and data[size] = data[size - 1] = 0;
       * @return true on success and false otherwise
       */
      bool parseZeroCopy(char * data, size_t size);

      // mandatory stuff
      Method                                            method_;
      uint8_t                                           proto_major_;
      uint8_t                                           proto_minor_;
      std::string                                       raw_location_;
      std::string                                       host_;

      // cleaned up location and queries
      std::string                                       location_;
      std::unordered_multimap<std::string, std::string> query_;

      // basic stuff
      int                                               accept_encoding_; // Coding bitfield
      bool                                              keep_alive_; // keep the connection ?

      // mmmiiam cookies :^)
      std::unordered_multimap<std::string, std::string> cookies_;

      // post and put details
      uint64_t                                          content_length_;
      std::string                                       content_type_;
      stream::Stream::Ptr                               body_;

      // usefull for stats
      std::string                                       referrer_;
      std::string                                       user_agent_;

      // other headers
      std::unordered_multimap<std::string, std::string> unparsed_headers_;
    };
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_HH */
