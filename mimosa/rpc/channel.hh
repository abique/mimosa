#ifndef MIMOSA_RPC_CHANNEL_HH
# define MIMOSA_RPC_CHANNEL_HH

# include "../non-copyable.hh"
# include "../ref-countable.hh"
# include "../stream/buffer.hh"
# include "../stream/buffered-stream.hh"
# include "../channel.hh"
# include "../thread.hh"
# include "protocol.hh"
# include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    class Server;

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
              ServiceMap::ConstPtr        service_map = nullptr);
      ~Channel();

      void start();
      inline Status status() const { return status_; }
      void close();
      void wait();

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

      typedef mimosa::Channel<stream::Buffer::Ptr,
                              stream::Buffer::SlistPtr> write_queue_type;

      stream::BufferedStream::Ptr        stream_;
      ServiceMap::ConstPtr               service_map_;
      Mutex                              scalls_mutex_;
      std::map<uint32_t, BasicCall::Ptr> scalls_; // sent calls
      Mutex                              rcalls_mutex_;
      std::map<uint32_t, BasicCall::Ptr> rcalls_; // received calls
      Status                             status_;
      uint32_t                           next_tag_;
      Thread                             wthread_;
      Thread                             rthread_;
      write_queue_type                   write_queue_;
    };
  }
}

#endif /* !MIMOSA_RPC_CHANNEL_HH */
