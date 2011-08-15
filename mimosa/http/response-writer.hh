#ifndef MIMOSA_HTTP_RESPONSE_WRITER_HH
# define MIMOSA_HTTP_RESPONSE_WRITER_HH

# include "../stream/stream.hh"
# include "../stream/buffer.hh"
# include "response.hh"

namespace mimosa
{
  namespace http
  {
    /** This stream buffers output until sendHeader is called.
     * Then it writes directly to the underlying stream */
    class ResponseWriter : public Response,
                           public stream::Stream
    {
    public:
      MIMOSA_DEF_PTR(ResponseWriter);

      ResponseWriter(Response & response, stream::Stream::Ptr stream);
      ~ResponseWriter();

      /** Stream related stuff
       * @{ */
      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual bool flush(runtime::Time timeout);
      /** @} */

      /** Writes the header to the client.
       * If content_length_ is 0 then the content length is unspecified
       * and the connection will close after this response (keep_alive_
       * will be set to false).
       * Until you call sendResponseHeader, everything you write to
       * body is buffered, so when you finishes Response knows the
       * amount of data to be written and set content_length_ */
      bool sendHeader();

      uint64_t pendingWrite() const;

    private:
      Response &            response_;
      stream::Stream::Ptr   stream_;
      stream::Buffer::Slist buffers_;
      bool                  header_sent_;
      bool                  is_finished_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_WRITER_HH */
