#include "basic-call.hh"

namespace mimosa
{
  namespace rpc
  {
    BasicCall::BasicCall(google::protobuf::Message * request,
                         google::protobuf::Message * response)
      : request_(request),
        response_(response),
        mutex_(),
        condition_(),
        is_canceled_(false),
        is_finished_(false),
        service_id_(0),
        method_id_(0),
        tag_(0)
    {
    }

    BasicCall::~BasicCall()
    {
      Mutex::Locker locker(mutex_);
      delete request_;
      delete response_;
      request_  = nullptr;
      response_ = nullptr;
    }

    void
    BasicCall::wait()
    {
      if (is_finished_ || is_canceled_)
        return;

      Mutex::Locker locker(mutex_);
      if (is_finished_ || is_canceled_)
        return;
      condition_.wait(mutex_);
    }

    void
    BasicCall::timedWait(Time timeout)
    {
      if (is_finished_ || is_canceled_)
        return;

      Mutex::Locker locker(mutex_);
      if (is_finished_ || is_canceled_)
        return;
      condition_.timedWait(mutex_, timeout);
    }

    void
    BasicCall::cancel()
    {
      Mutex::Locker locker(mutex_);
      is_finished_ = true;
      is_canceled_ = true;
      condition_.wakeAll();
    }

    void
    BasicCall::finished()
    {
      Mutex::Locker locker(mutex_);
      is_finished_ = true;
      condition_.wakeAll();
    }
  }
}
