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
      ~Response();

      /** convert the response to an http response header */
      bool print(stream::Stream & stream) const;
      std::string toHttpHeader() const;

      /**
       * @{
       * Content-Range stuff.
       * http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.16
       *
       * contentRangeLength is set to -1 for '*'.
       */
      inline bool hasContentRange() const {
        return status_ == kStatusPartialContent;
      }
      inline int64_t contentRangeStart() const { return content_range_start_; }
      inline int64_t contentRangeEnd() const { return content_range_end_; }
      inline int64_t contentRangeLength() const { return content_range_length_; }
      inline void setContentRange(int64_t start, int64_t end, int64_t length) {
        content_range_start_  = start;
        content_range_end_    = end;
        content_range_length_ = length;
      }
      inline void setStatus(Status status) { status_ = status; }
      /** @} */

      inline void setContentType(const std::string & ct) { content_type_ = ct; }

      void clear();

      Status        status_;
      bool          keep_alive_;
      Coding        content_encoding_;
      Coding        transfer_encoding_;
      int64_t       content_length_; ///< -1: auto
      std::string   content_type_;
      Cookie::Slist cookies_;
      kvs           unparsed_headers_;
      time_t        last_modified_; // local time

      // content-range
      int64_t content_range_start_;
      int64_t content_range_end_;
      int64_t content_range_length_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_HH */
