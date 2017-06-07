#pragma once

#include "../stream/stream.hh"

namespace mimosa
{
  namespace http
  {
    class RequestWriter;

    /**
     * @ingroup Http
     */
    template <typename Channel, typename Message>
    class MessageReader : public stream::Stream,
                          public Message
    {
    public:
      MIMOSA_DEF_PTR(MessageReader);

      MessageReader(Channel & channel);

      void clear();

      /** Stream related stuff
       * @{ */
      /** @warning this should never be called, will abort */
      virtual int64_t write(const char * data, uint64_t nbytes);
      /** reads the body (PUT and POST) */
      virtual int64_t read(char * data, uint64_t nbytes);
      /** reads and discards all remaining body data */
      virtual bool flush();
      /** @} */

      inline Channel & channel() const { return channel_; }

    private:

      friend Channel;
      friend RequestWriter;

      bool prepare();

      Channel &           channel_;
      stream::Stream::Ptr stream_;
      int64_t             bytes_left_;
    };
  }
}
