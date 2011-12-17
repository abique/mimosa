#ifndef MIMOSA_HTTP_REQUEST_HH
# define MIMOSA_HTTP_REQUEST_HH

# include <vector>
# include <map>

# include "../container/kvs.hh"
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

      void clear();

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

      inline const std::string & host() const { return host_; }
      inline void setHost(const std::string & s) { host_ = s; }

      inline uint16_t port() const { return port_; }
      inline void setPort(uint16_t port) { port_ = port; }

      const std::string & location() const;
      const container::kvs & query() const;

      int acceptEncoding() const { return accept_encoding_; }
      void setAcceptEncoding(int v) { accept_encoding_ = v; }

      bool keepAlive() const { return keep_alive_; }
      void setKeepAlive(bool v) { keep_alive_ = v; }

      const container::kvs & cookies() const { return cookies_; }
      inline void addCookie(const std::string & key,
                            const std::string & value)
      {
        cookies_.insert(std::make_pair(key, value));
      }

      inline int64_t contentLength() const { return content_length_; }
      inline void setContentLength(int64_t v) { content_length_ = v; }

      inline const std::string & contentType() const { return content_type_; }
      inline void setContentType(const std::string & s) { content_type_ = s; }

      inline stream::Stream::Ptr body() const { return body_; }
      inline void setBody(stream::Stream::Ptr stream) { body_ = stream; }

      inline const std::string & userAgent() const { return user_agent_; }
      inline void setUserAgent(const std::string & s) { user_agent_ = s; }

      inline const std::string & referrer() const { return referrer_; }
      inline void setReferrer(const std::string & s) { referrer_ = s; }

      inline const container::kvs & unparsedHeaders() const { return unparsed_headers_; }
      inline void addHeader(const std::string & key,
                            const std::string & value)
      {
        unparsed_headers_.insert(std::make_pair(key, value));
      }

    protected:

      // mandatory stuff
      Method      method_;
      uint8_t     proto_major_;
      uint8_t     proto_minor_;
      std::string raw_location_;
      std::string host_;
      uint16_t    port_;

      // cleaned up location and queries
      mutable bool           location_normalized_;
      mutable std::string    location_;
      mutable bool           query_parsed_;
      mutable container::kvs query_;

      // basic stuff
      int  accept_encoding_;    // Coding bitfield
      bool keep_alive_;         // keep the connection ?

      // mmmiiam cookies :^)
      container::kvs cookies_;

      // post and put details
      int64_t             content_length_;
      std::string         content_type_;
      stream::Stream::Ptr body_;

      // usefull for stats
      std::string referrer_;
      std::string user_agent_;

      // other headers
      container::kvs unparsed_headers_;
    };
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_HH */
