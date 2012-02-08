#ifndef MIMOSA_HTTP_RESPONSE_WRITER_HH
# define MIMOSA_HTTP_RESPONSE_WRITER_HH

# include "../stream/stream.hh"
# include "../stream/buffer.hh"
# include "response.hh"

namespace mimosa
{
  namespace http
  {
    class ServerChannel;

    /** This stream buffers output until sendHeader is called.
     * Then it writes directly to the underlying stream */
    class ResponseWriter : public stream::Stream,
                           public Response
    {
    public:
      MIMOSA_DEF_PTR(ResponseWriter);

      ResponseWriter(ServerChannel & channel,
                     runtime::Time   write_timeout);
      ~ResponseWriter();

      /** Stream related stuff
       * @{ */
      /** buffers the data until sendHeader() */
      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      /** @warning this should never be called, will abort */
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      /** does nothing until sendHeader(), then flushes the write buffer */
      virtual bool flush(runtime::Time timeout = 0);
      /** @} */

      /** Writes the header to the client.
       * If content_length_ is 0 then the content length is unspecified
       * and the connection will close after this response (keep_alive_
       * will be set to false).
       * Until you call sendResponseHeader, everything you write to
       * body is buffered, so when you finishes Response knows the
       * amount of data to be written and set content_length_ */
      bool sendHeader(runtime::Time timeout = 0);

      void clear();

      inline runtime::Time writeTimeout() const
      {
        return write_timeout_ > 0 ? runtime::time() + write_timeout_ : 0;
      }

      inline ServerChannel & channel() const { return channel_; }

    private:
      friend class ServerChannel;

      /** tells that you finished to modify ResponseWriter and you will not
       * call write(). This method should only be called by ServerChannel. */
      bool finish(runtime::Time timeout);
      /** body length (to be written) */
      uint64_t pendingWrite() const;

      int64_t writeChunk(const char *  data,
                         uint64_t      nbytes,
                         runtime::Time timeout);

      ServerChannel &       channel_;
      stream::Buffer::Slist buffers_;
      bool                  header_sent_;
      runtime::Time         write_timeout_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_WRITER_HH */
