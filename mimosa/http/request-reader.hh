#ifndef MIMOSA_HTTP_REQUEST_READER_HH
# define MIMOSA_HTTP_REQUEST_READER_HH

# include "request.hh"
# include "../stream/stream.hh"
# include "message-reader.hh"

namespace mimosa
{
  namespace http
  {
    class  ServerChannel;
    typedef MessageReader<ServerChannel, Request> RequestReader;
    extern template class MessageReader<ServerChannel, Request>;
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_READER_HH */
