#ifndef MIMOSA_HTTP_RESPONSE_READER_HH
# define MIMOSA_HTTP_RESPONSE_READER_HH

# include "response.hh"
# include "message-reader.hh"

namespace mimosa
{
  namespace http
  {
    class ClientChannel;
    class Request;

    typedef MessageReader<ClientChannel, Response> ResponseReader;
    extern template class MessageReader<ClientChannel, Response>;
  }
}

#endif /* !MIMOSA_HTTP_RESPONSE_READER_HH */
