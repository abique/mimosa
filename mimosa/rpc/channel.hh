#ifndef MIMOSA_RPC_CHANNEL_HH
# define MIMOSA_RPC_CHANNEL_HH

# include "../non-copyable.hh"
# include "../ref-countable.hh"
# include "../stream/buffer.hh"
# include "../stream/buffered-stream.hh"
# include "../sync/channel.hh"
# include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    class Channel : public RefCountable<Channel>,
                    private NonCopyable
    {
    public:
      Channel(stream::BufferedStream::Ptr stream,
              ServiceMap::Ptr             service_map);

      void close();

      enum Status
      {
        kOk,
        kClosed,
      };

    private:
      void readLoop();
      void writeLoop();

      void handleCall();
      void handleResult();
      void handleError();

      stream::BufferedStream::Ptr                               stream_;
      ServiceMap::Ptr                                           service_map_;
      std::map<uint32_t, BasicCall::Ptr>                        sent_calls_;
      std::map<uint32_t, BasicCall::Ptr>                        received_calls_;
      Status                                                    status_;
      sync::Channel<stream::Buffer::Ptr, stream::Buffer::Slist> write_queue_;
    };
  }
}

#endif /* !MIMOSA_RPC_CHANNEL_HH */
