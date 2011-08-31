#include "../runtime/fiber.hh"
#include "channel.hh"
#include "protocol.hh"

namespace mimosa
{
  namespace rpc
  {
    Channel::Channel(stream::BufferedStream::Ptr stream,
                     ServiceMap::Ptr             service_map)
      : stream_(stream),
        service_map_(service_map),
        sent_calls_(),
        received_calls_(),
        status_(kOk),
        write_queue_()
    {
      Channel::Ptr channel(this);
      runtime::Fiber::start([channel]() { channel->readLoop(); });
      runtime::Fiber::start([channel]() { channel->writeLoop(); });
    }

    void
    Channel::writeLoop()
    {
      while (status_ == kOk)
      {
        stream::Buffer::Ptr buffer;
        if (!write_queue_.pop(buffer))
        {
          stream_->flush();
          continue;
        }
        if (stream_->write(buffer->data(), buffer->size()) != buffer->size())
        {
          status_ = kClosed;
          return;
        }
      }
    }

    void
    Channel::readLoop()
    {
      while (status_ == kOk)
      {
        uint8_t msg_type;
        if (stream_->read((char*)&msg_type, 1) != 1)
        {
          status_ = kClosed;
          return;
        }

        switch (msg_type)
        {
        case kClose: status_ = kClosed; return;
        case kCall:   handleCall(); break;
        case kResult: handleResult(); break;
        case kError:  handleError(); break;
        default: status_ = kClosed; return; // TODO
        }
      }
    }
  }
}
