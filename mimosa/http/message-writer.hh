#pragma once

# include "../stream/direct-fd-stream.hh"
# include "../stream/buffer.hh"

namespace mimosa
{
  namespace http
  {
    class Request;

    /**
     * This stream buffers output until sendHeader is called.
     * Then it writes directly to the underlying stream
     * XXX Remove internal buffer, and only support HTTP/1.1
     *
     * @ingroup Http
     */
    template <typename Channel, typename Message>
    class MessageWriter : public stream::Stream,
                          public Message
    {
    public:
      MIMOSA_DEF_PTR(MessageWriter<Channel, Message>);

      MessageWriter(Channel & channel);
      ~MessageWriter();

      /** Stream related stuff
       * @{ */

      /** If called before sendHeader(), sends the headers and
       * set transfer_encoding_ to kCodingChunked. */
      virtual int64_t write(const char * data, uint64_t nbytes);
      /** @warning this should never be called, will abort */
      virtual int64_t read(char * data, uint64_t nbytes);
      /** does nothing until sendHeader(), then flushes the write buffer */
      virtual bool flush();

      /** @} */

      /** Writes the header to the client.
       * If content_length_ is 0 then the content length is unspecified
       * and the connection will close after this response (keep_alive_
       * will be set to false).
       * Until you call sendHeader, everything you write to
       * body is buffered, so when you finishes Response knows the
       * amount of data to be written and set content_length_ */
      bool sendHeader();

      void clear();

      /** Will try to enable compression depending on what the client
       * supports. Compression will be transparent for the user, simply keep
       * using response.write().
       *
       * @return true if compression is enabled, false otherwise.
       */
      bool enableCompression(const Request & request);

      inline Channel & channel() const { return channel_; }

      /** @return nullptr if we can't convert the output stream to a direct fd */
      stream::DirectFdStream * directFdStream();

    protected:
      friend Channel;

      /** tells that you finished to modify ResponseWriter and you will not
       * call write(). This method should only be called by ServerChannel. */
      bool finish();

      Channel &             channel_;
      Stream::Ptr           stream_;
      stream::Buffer::Slist buffers_;
      bool                  header_sent_;
    };
  }
}

