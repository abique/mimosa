#ifndef MIMOSA_HTTP_REQUEST_WRITER_HH
# define MIMOSA_HTTP_REQUEST_WRITER_HH

# include "request.hh"
# include "message-writer.hh"

namespace mimosa
{
  namespace http
  {
    class  ClientChannel;
    typedef MessageWriter<ClientChannel, Request> RequestWriter;
    extern template class MessageWriter<ClientChannel, Request>;
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_WRITER_HH */
