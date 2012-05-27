#ifndef MIMOSA_HTTP_RESPONSE_WRITER_HH
# define MIMOSA_HTTP_RESPONSE_WRITER_HH

# include "../stream/stream.hh"
# include "../stream/direct-fd-stream.hh"
# include "../stream/buffer.hh"
# include "response.hh"

namespace mimosa
{
  namespace http
  {
    class ServerChannel;

    /**
     * This stream buffers output until sendHeader is called.
     * Then it writes directly to the underlying stream
     * XXX Remove internal buffer, and only support HTTP/1.1
     *
     * @ingroup Http
     */
    class ResponseWriter : public stream::Stream,
                           public Response
    {
    public:
      MIMOSA_DEF_PTR(ResponseWriter);

      ResponseWriter(ServerChannel & channel,
                     Time            write_timeout);
      ~ResponseWriter();

      /** Stream related stuff
       * @{ */

      /** If called before sendHeader(), sends the headers and
       * set transfer_encoding_ to kCodingChunked. */
      virtual int64_t write(const char * data, uint64_t nbytes, Time timeout = 0);
      /** @warning this should never be called, will abort */
      virtual int64_t read(char * data, uint64_t nbytes, Time timeout = 0);
      /** does nothing until sendHeader(), then flushes the write buffer */
      virtual bool flush(Time timeout = 0);

      /** @} */

      /** Writes the header to the client.
       * If content_length_ is 0 then the content length is unspecified
       * and the connection will close after this response (keep_alive_
       * will be set to false).
       * Until you call sendHeader, everything you write to
       * body is buffered, so when you finishes Response knows the
       * amount of data to be written and set content_length_ */
      bool sendHeader(Time timeout = 0);

      void clear();

      inline Time writeTimeout() const
      {
        return write_timeout_ > 0 ? time() + write_timeout_ : 0;
      }

      inline ServerChannel & channel() const { return channel_; }

      /** @return nullptr if we can't convert the output stream to a direct fd */
      stream::DirectFdStream * directFdStream();

    private:
      friend class ServerChannel;

      /** tells that you finished to modify ResponseWriter and you will not
       * call write(). This method should only be called by ServerChannel. */
      bool finish(Time timeout);

      /** writes a chunk if transfer_encoding_ is kCodingChunked */
      int64_t writeChunk(const char *  data,
                         uint64_t      nbytes,
                         Time timeout);

      ServerChannel &       channel_;
      stream::Buffer::Slist buffers_;
      bool                  header_sent_;
      Time                  write_timeout_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_WRITER_HH */
