#ifndef MIMOSA_RPC_CHANNEL_HH
# define MIMOSA_RPC_CHANNEL_HH

# include "../non-copyable.hh"
# include "../ref-countable.hh"
# include "../stream/buffer.hh"
# include "../stream/buffered-stream.hh"
# include "../sync/channel.hh"
# include "protocol.hh"
# include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    class Channel : public RefCountable<Channel>,
                    private NonCopyable
    {
    public:
      enum Status
      {
        kOk,
        kClosed,
      };

      Channel(stream::BufferedStream::Ptr stream,
              ServiceMap::Ptr             service_map);

      inline Status status() const { return status_; }
      void close();

      void sendCall(BasicCall::Ptr call);
      void sendResponse(BasicCall::Ptr call);
      void sendError(ErrorType error, uint32_t tag, TagOrigin tag_origin);

    private:
      /** internal read loop */
      void readLoop();
      /** internal write loop */
      void writeLoop();

      /** @return true on success, false on fatal error which implies closing
       * the channel.
       * @{
       */
      bool handleCall();
      bool handleResult();
      bool handleError();
      /** @} */

      /** generate a tag.
       * @warning you still have to check that the tag is not in use and if it is
       * then re-call nextTag() */
      uint32_t nextTag();

      stream::BufferedStream::Ptr                               stream_;
      ServiceMap::Ptr                                           service_map_;
      sync::Mutex                                               scalls_mutex_;
      std::map<uint32_t, BasicCall::Ptr>                        scalls_; // sent calls
      sync::Mutex                                               rcalls_mutex_;
      std::map<uint32_t, BasicCall::Ptr>                        rcalls_; // received calls
      Status                                                    status_;
      sync::Channel<stream::Buffer::Ptr, stream::Buffer::Slist> write_queue_;
      uint32_t                                                  next_tag_;
    };
  }
}

#endif /* !MIMOSA_RPC_CHANNEL_HH */
