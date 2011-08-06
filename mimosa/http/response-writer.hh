#ifndef MIMOSA_HTTP_RESPONSE_WRITER_HH
# define MIMOSA_HTTP_RESPONSE_WRITER_HH

# include "../stream/stream.hh"
# include "../stream/buffer.hh"
# include "response.hh"

namespace mimosa
{
  namespace http
  {
    /** this stream buffers output until is_response_header_sent_ is true.
     * then it writes directly to the underlying stream */
    class ResponseWriter : public stream::Stream
    {
    public:
      ResponseWriter(Response & response, stream::Stream::Ptr stream);
      ~ResponseWriter();

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual bool flush(runtime::Time timeout);

      uint64_t pendingWrite() const;

    private:
      Response &            response_;
      stream::Stream::Ptr   stream_;
      stream::Buffer::Slist buffers_;
    };
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_WRITER_HH */
