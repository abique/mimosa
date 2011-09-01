#include <endian.h>

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
        scalls_(),
        rcalls_(),
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
        sync::Mutex::Locker locker(scalls_mutex_);
        while (true)
        {
          auto it = scalls_.find(call->tag());
          if (it == scalls_.end())
            break;
          call->setTag(nextTag());
          msg->tag_ = htole32(call->tag());
        }
        scalls_[call->tag()] = call;
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
        if (stream_->loopWrite(buffer->data(), buffer->size()) != buffer->size())
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

    void
    Channel::handleCall()
    {
      MsgCall msg;
      char *  data = 1 + (char *)&msg;
      if (stream_->loopRead(data, sizeof (msg) - 1) != sizeof (msg) - 1)
      {
        status_ = kClosed;
        return;
      }
      msg.tag_        = le32toh(msg.tag_);
      msg.service_id_ = le32toh(msg.service_id_);
      msg.method_id_  = le32toh(msg.method_id_);
      msg.rq_size_    = le32toh(msg.rq_size_);

      BasicCall::Ptr call = new BasicCall();
      call->setTag(msg.tag_);
      call->setServiceId(msg.service_id_);
      call->setMethodId(msg.method_id_);

      // find service
      auto service = service_map_->find(call->serviceId());
      if (!service)
      {
        error(kServiceNotFound, call->tag());
        return;
      }

      // check for duplicate tag
      {
        sync::Mutex::Locker locker(rcalls_mutex_);
        auto it = rcalls_.find(msg.tag_);
        if (it != rcalls_.end())
        {
          error(kDuplicateTag, call->tag());
          return;
        }
        rcalls_[call->tag()] = call;
      }

      // call method
      data = (char *)::malloc(msg.rq_size_);
      if (!data)
      {
        error(kInternalError, call->tag());
        sync::Mutex::Locker locker(rcalls_mutex_);
        rcalls_.erase(call->tag());
        return;
      }
      if (stream_->loopRead(data, msg.rq_size_) != msg.rq_size_)
      {
        status_ = kClosed;
        return;
      }

      Channel::Ptr channel(this);
      auto data_size = msg.rq_size_;
      runtime::Fiber::start([channel, call, data, data_size, service] () {
          auto ret = service->callMethod(call, data, data_size);
          free(data);
          if (ret != Service::kSucceed)
            channel->error(static_cast<ErrorType> (ret), call->tag());
          else
            channel->sendResponse(call);
          sync::Mutex::Locker locker(channel->rcalls_mutex_);
          channel->rcalls_.erase(call->tag());
        });
    }
  }
}
