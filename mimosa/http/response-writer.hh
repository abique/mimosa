#ifndef MIMOSA_HTTP_RESPONSE_WRITER_HH
# define MIMOSA_HTTP_RESPONSE_WRITER_HH

# include "../stream/stream.hh"
# include "../stream/direct-fd-stream.hh"
# include "../stream/buffer.hh"
# include "response.hh"
# include "message-writer.hh"

namespace mimosa
{
  namespace http
  {
    class ServerChannel;
    class Request;

    typedef MessageWriter<ServerChannel, Response> ResponseWriter;
    extern template class MessageWriter<ServerChannel, Response>;
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_WRITER_HH */
