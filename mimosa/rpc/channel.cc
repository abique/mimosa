#include <cassert>
#include <memory>
#include <utility>
#include <utility>

#include "../endian.hh"
#include "../thread.hh"
#include "channel.hh"
#include "protocol.hh"
#include "log.hh"

namespace mimosa
{
  namespace rpc
  {
    Channel::Channel(stream::BufferedStream::Ptr stream,
                     ServiceMap::ConstPtr        service_map)
      : stream_(std::move(std::move(stream))),
        service_map_(std::move(std::move(service_map))),
        scalls_(),
        rcalls_(),
        status_(kOk),
        next_tag_(0),
        wthread_([this] { this->writeLoop(); }),
        rthread_([this] { this->readLoop(); }),
        write_queue_()
    {
    }

    Channel::~Channel()
    {
      close();
    }

    uint32_t
    Channel::nextTag()
    {
      return __sync_add_and_fetch(&next_tag_, 1);
    }

    void
    Channel::start()
    {
      wthread_.start();
      rthread_.start();
    }

    void
    Channel::writeLoop()
    {
      while (status_ == kOk)
      {
        stream::Buffer::Ptr buffer;

        if (!write_queue_.pop(buffer))
          return;

        assert(buffer);
        if (stream_->loopWrite(buffer->data(), buffer->size()) !=
            static_cast<int64_t> (buffer->size())) {
          stream_->close();
          return;
        }

        if (!stream_->flush())
          return;
      }
    }

    void
    Channel::readLoop()
    {
      while (status_ == kOk)
      {
        uint8_t msg_type;
        int ret = 0;
        if ((ret = stream_->read((char*)&msg_type, 1)) != 1) {
          stream_->close();
          return;
        }

        bool ok = false;
        switch (msg_type)
        {
        case kCall:   ok = handleCall(); break;
        case kResult: ok = handleResult(); break;
        case kError:  ok = handleError(); break;

        case kClose:  ok = false; break;
        default:      ok = false; break;
        }

        if (!ok) {
          stream_->close();
          return;
        }
      }
    }

    void
    Channel::sendCall(const BasicCall::Ptr& call)
    {
      call->setTag(nextTag());

      uint32_t rq_size = call->request_->ByteSize();
      stream::Buffer::Ptr buffer = new stream::Buffer(rq_size + sizeof (MsgCall));
      auto * msg    = reinterpret_cast<MsgCall *> (buffer->data());
      msg->type_       = kCall;
      msg->tag_        = htole32(call->tag());
      msg->service_id_ = htole32(call->serviceId());
      msg->method_id_  = htole32(call->methodId());
      msg->rq_size_    = htole32(rq_size);
      call->request_->SerializeToArray(msg->rq_, rq_size);
      {
        Mutex::Locker locker(scalls_mutex_);
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
      if (!write_queue_.push(buffer))
      {
        Mutex::Locker locker(scalls_mutex_);
        scalls_.erase(call->tag());
        call->cancel();
        rpc_log->critical("could not push the buffer into the queue");
      }
    }

    void
    Channel::sendResponse(const BasicCall::Ptr& call)
    {
      uint32_t rp_size  = call->response_->ByteSize();
      stream::Buffer::Ptr buffer = new stream::Buffer(rp_size + sizeof (MsgResult));
      auto * msg  = reinterpret_cast<MsgResult *> (buffer->data());
      msg->type_       = kResult;
      msg->tag_        = htole32(call->tag());
      msg->rp_size_    = htole32(rp_size);
      call->response_->SerializeToArray(msg->rp_, rp_size);
      write_queue_.push(buffer);

      Mutex::Locker locker(rcalls_mutex_);
      rcalls_.erase(call->tag());
    }

    void
    Channel::sendError(ErrorType error, uint32_t tag, TagOrigin tag_origin)
    {
      stream::Buffer::Ptr buffer = new stream::Buffer(sizeof (MsgError));
      auto * msg   = reinterpret_cast<MsgError *> (buffer->data());
      msg->type_       = kError;
      msg->error_      = error;
      msg->tag_        = htole32(tag);
      msg->tag_origin_ = tag_origin;
      write_queue_.push(buffer);

      Mutex::Locker locker(rcalls_mutex_);
      rcalls_.erase(tag);
    }

    bool
    Channel::handleCall()
    {
      MsgCall msg;
      char *  data = 1 + (char *)&msg.type_;
      if (stream_->loopRead(data, sizeof (msg) - 1) != sizeof (msg) - 1)
        return false;

      msg.tag_        = le32toh(msg.tag_);
      msg.service_id_ = le32toh(msg.service_id_);
      msg.method_id_  = le32toh(msg.method_id_);
      msg.rq_size_    = le32toh(msg.rq_size_);

      BasicCall::Ptr call = new BasicCall();
      call->setTag(msg.tag_);
      call->setServiceId(msg.service_id_);
      call->setMethodId(msg.method_id_);

      // read the end of the message
      data = (char *)::malloc(msg.rq_size_);
      if (!data) {
        sendError(kInternalError, call->tag(), kOriginYou);
        rpc_log->critical("failed to allocate %d bytes", msg.rq_size_);
        return false;
      }

      if (stream_->loopRead(data, msg.rq_size_) != msg.rq_size_)
      {
        free(data);
        sendError(kInternalError, call->tag(), kOriginYou);
        return false;
      }

      // find service
      auto service = service_map_->find(call->serviceId());
      if (!service)
      {
        free(data);
        sendError(kServiceNotFound, call->tag(), kOriginYou);
        rpc_log->warning("received method call for unknown service: %d", call->serviceId());
        return true;
      }

      // check for duplicate tag
      {
        Mutex::Locker locker(rcalls_mutex_);
        auto it = rcalls_.find(msg.tag_);
        if (it != rcalls_.end())
        {
          free(data);
          sendError(kDuplicateTag, call->tag(), kOriginYou);
          rpc_log->warning("got duplicate tag: %d", msg.tag_);
          return true;
        }
        rcalls_[call->tag()] = call;
      }

      // call method
      Channel::Ptr channel(this);
      auto data_size = msg.rq_size_;
      Thread([channel, call, data, data_size, service] () {
          auto ret = service->callMethod(call, data, data_size);
          free(data);
          if (ret != Service::kSucceed)
            channel->sendError(static_cast<ErrorType> (ret), call->tag(), kOriginYou);
          else
            channel->sendResponse(call);
        }).start();
      return true;
    }

    bool
    Channel::handleResult()
    {
      MsgResult msg;
      char *    data = 1 + (char *)&msg.type_;
      if (stream_->loopRead(data, sizeof (msg) - 1) != sizeof (msg) - 1)
        return false;
      msg.tag_     = le32toh(msg.tag_);
      msg.rp_size_ = le32toh(msg.rp_size_);

      data = (char *)::malloc(msg.rp_size_);
      if (!data)
        return false;
      if (stream_->loopRead(data, msg.rp_size_) != msg.rp_size_)
      {
        free(data);
        return false;
      }

      BasicCall::Ptr call;
      {
        Mutex::Locker locker(scalls_mutex_);
        auto it = scalls_.find(msg.tag_);
        if (it == scalls_.end())
        {
          free(data);
          return true;
        }
        call = it->second;
        scalls_.erase(it);
      }
      call->response_->ParseFromArray(data, msg.rp_size_);
      free(data);
      call->finished();
      return true;
    }

    bool
    Channel::handleError()
    {
      MsgError msg;
      char *   data = 1 + (char *)&msg;
      if (stream_->loopRead(data, sizeof (msg) - 1) != sizeof (msg) - 1)
        return false;
      msg.tag_ = le32toh(msg.tag_);

      switch (msg.tag_origin_)
      {
      case kOriginYou:
      {
        Mutex::Locker locker(scalls_mutex_);
        auto it = scalls_.find(msg.tag_);
        if (it == scalls_.end())
          return true;
        it->second->cancel();
        scalls_.erase(it);
        return true;
      }

      case kOriginMe:
      {
        Mutex::Locker locker(rcalls_mutex_);
        auto it = rcalls_.find(msg.tag_);
        if (it == rcalls_.end())
          return true;
        it->second->cancel();
        rcalls_.erase(it);
        return true;
      }

      default:
        // TODO ????
        return true;
      }
    }

    void
    Channel::wait()
    {
      rthread_.join();
    }

    void
    Channel::close()
    {
      status_ = kClosed;
      stream_->shutdown();
      write_queue_.close();
      wthread_.join();
      rthread_.cancel();
      rthread_.join();
      stream_->close();
    }
  }
}
