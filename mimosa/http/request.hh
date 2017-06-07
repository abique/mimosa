#pragma once

# include <vector>
# include <map>

# include "../kvs.hh"
# include "../uri/url.hh"
# include "../stream/stream.hh"
# include "coding.hh"
# include "method.hh"
# include "byte-range.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class Request
    {
    public:

      Request();

      void clear();

      /** convert the response to an http response header */
      bool print(stream::Stream & stream) const;
      std::string toHttpHeader() const;

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

      inline Method method() const { return method_; }
      inline void setMethod(Method m) { method_ = m; }

      inline uint8_t protoMajor() const { return proto_major_; }
      inline uint8_t protoMinor() const { return proto_minor_; }
      inline void setProto(uint8_t major, uint8_t minor) { proto_major_ = major; proto_minor_ = minor; }

      inline const std::string & rawLocation() const { return raw_location_; }
      inline void setRawLocation(const std::string & s) { raw_location_ = s; }
      inline void setRawLocation(std::string && s) { raw_location_ = s; }

      inline const std::string & host() const { return host_; }
      inline void setHost(const std::string & s) { host_ = s; }
      inline void setHost(std::string && s) { host_ = s; }

      inline uint16_t port() const { return port_; }
      inline void setPort(uint16_t port) { port_ = port; }

      const std::string & location() const;
      const kvs & query() const;
      const std::string & queryGet(const std::string & key) const;

      int acceptEncoding() const { return accept_encoding_; }
      void setAcceptEncoding(int v) { accept_encoding_ = v; }

      bool keepAlive() const { return keep_alive_; }
      void setKeepAlive(bool v) { keep_alive_ = v; }

      const kvs & cookies() const { return cookies_; }
      inline void addCookie(const std::string & key,
                            const std::string & value) {
        cookies_.insert(std::make_pair(key, value));
      }

      inline int64_t contentLength() const { return content_length_; }
      inline void setContentLength(int64_t v) { content_length_ = v; }

      inline const std::string & contentType() const { return content_type_; }
      inline void setContentType(const std::string & s) { content_type_ = s; }
      inline void setContentType(std::string && s) { content_type_ = s; }

      inline stream::Stream::Ptr body() const { return body_; }
      inline void setBody(stream::Stream::Ptr stream) { body_ = stream; }

      inline const std::string & userAgent() const { return user_agent_; }
      inline void setUserAgent(const std::string & s) { user_agent_ = s; }
      inline void setUserAgent(std::string && s) { user_agent_ = s; }

      inline const std::string & referrer() const { return referrer_; }
      inline void setReferrer(const std::string & s) { referrer_ = s; }
      inline void setReferrer(std::string && s) { referrer_ = s; }

      inline const kvs & unparsedHeaders() const { return unparsed_headers_; }
      inline void addHeader(const std::string & key,
                            const std::string & value) {
        unparsed_headers_.insert(std::make_pair(key, value));
      }

      inline time_t hasIfModifiedSince() const { return if_modified_since_ > 0; }
      inline time_t ifModifiedSince() const { return if_modified_since_; }
      void parseIfModifiedSince(const std::string & value);

      /**
       * @{
       * Content-Range stuff.
       * http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.16
       *
       * contentRangeLength is set to -1 for '*'.
       */
      inline bool hasContentRange() const {
        return content_range_start_ > 0 || content_range_end_ > 0;
      }
      inline int64_t contentRangeStart() const { return content_range_start_; }
      inline int64_t contentRangeEnd() const { return content_range_end_; }
      inline int64_t contentRangeLength() const { return content_range_length_; }
      inline void setContentRange(int64_t start, int64_t end, int64_t length) {
        content_range_start_  = start;
        content_range_end_    = end;
        content_range_length_ = length;
      }
      /** @} */

      inline Coding contentEncoding() const { return content_encoding_; }
      inline void setContentEncoding(Coding c) { content_encoding_ = c; }
      inline Coding transferEncoding() const { return transfer_encoding_; }
      inline void setTransferEncoding(Coding c) { transfer_encoding_ = c; }

      inline void setDestination(const std::string & dest) { destination_ = dest; }
      const std::string & destination() const;

      // sets also raw location, host, port and location
      void setUrl(const uri::Url & url);
      inline const uri::Url & url() const { return url_; }

      bool hasRange() const { return !ranges_.empty(); }
      const std::vector<ByteRange> & range() const { return ranges_; }
      void addRange(const ByteRange & r) { ranges_.push_back(r); }

    protected:
      // mandatory stuff
      Method      method_;
      uint8_t     proto_major_;
      uint8_t     proto_minor_;
      std::string raw_location_;
      std::string host_;
      uint16_t    port_;
      time_t      if_modified_since_; // local time

      // requiered when sending a request
      uri::Url url_;

      // cleaned up location and queries
      mutable bool        location_normalized_;
      mutable std::string location_;
      mutable bool        query_parsed_;
      mutable kvs         query_;

      // basic stuff
      int  accept_encoding_;    // Coding bitfield
      bool keep_alive_;         // keep the connection ?

      // mmmiiam cookies :^)
      kvs cookies_;

      // post and put details
      int64_t             content_length_;
      std::string         content_type_;
      stream::Stream::Ptr body_;

      // usefull for stats
      std::string referrer_;
      std::string user_agent_;

      // content-range
      int64_t content_range_start_;
      int64_t content_range_end_;
      int64_t content_range_length_;

      std::vector<ByteRange> ranges_;

      Coding content_encoding_;
      Coding transfer_encoding_;

      mutable bool destination_normalized_;
      mutable std::string destination_;

      // other headers
      kvs unparsed_headers_;
    };
  }
}

