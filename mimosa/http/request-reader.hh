#ifndef MIMOSA_HTTP_REQUEST_READER_HH
# define MIMOSA_HTTP_REQUEST_READER_HH

# include "request.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace http
  {
    class RequestReader : public Request,
                          public stream::Stream
    {
    public:

      MIMOSA_DEF_PTR(RequestReader);

      /** Stream related stuff
       * @{ */
      /** @warning this should never be called, will abort */
      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      /** reads the body (PUT and POST) */
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      /** @} */

    };
  }
}

#endif /* !MIMOSA_HTTP_REQUEST_READER_HH */
