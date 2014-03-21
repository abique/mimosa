#ifndef MIMOSA_HTTP_REQUEST_WRITER_HH
# define MIMOSA_HTTP_REQUEST_WRITER_HH

# include "request.hh"
# include "message-writer.hh"
# include "response-reader.hh"

namespace mimosa
{
  namespace http
  {
    class ClientChannel;

    class RequestWriter : public MessageWriter<ClientChannel, Request>
    {
    public:
      inline RequestWriter(ClientChannel & channel)
        : MessageWriter<ClientChannel, Request> (channel)
      {
      }

      ResponseReader::Ptr send();
    };

    extern template class MessageWriter<ClientChannel, Request>;
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_WRITER_HH */
