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
        write_queue_(),
        next_tag_(0)
    {
      Channel::Ptr channel(this);
      runtime::Fiber::start([channel]() { channel->readLoop(); });
      runtime::Fiber::start([channel]() { channel->writeLoop(); });
    }

    uint32_t
    Channel::nextTag()
    {
      return __sync_add_and_fetch(&next_tag_, 1);
    }

    void
    Channel::callMethod(BasicCall::Ptr call)
    {
      call->setTag(nextTag());

      uint32_t rq_size  = call->request_->ByteSize();
      stream::Buffer::Ptr buffer = new stream::Buffer(rq_size + sizeof (MsgCall));
      MsgCall * msg = reinterpret_cast<MsgCall *> (buffer->data());
      msg->type_       = kCall;
      msg->tag_        = htole32(call->tag());
      msg->service_id_ = htole32(call->serviceId());
      msg->method_id_  = htole32(call->methodId());
      msg->rq_size_    = htole32(rq_size);
      call->request_->SerializeToArray(msg->rq_, rq_size);
      {
        sync::Mutex::Locker locker(sent_calls_mutex_);
        while (true)
        {
          auto it = sent_calls_.find(call->tag());
          if (it == sent_calls_.end())
            break;
          call->setTag(nextTag());
          msg->tag_ = htole32(call->tag());
        }
        sent_calls_[call->tag()] = call;
      }
      write_queue_.push(buffer);
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
