#pragma once

#include <google/protobuf/message.h>

#include "../ref-countable.hh"
#include "../mutex.hh"
#include "../condition.hh"

namespace mimosa
{
  namespace rpc
  {
    class Channel;

    class BasicCall : public RefCountable<BasicCall>, private NonCopyable, private NonMovable
    {
    public:
      explicit BasicCall(google::protobuf::Message * request  = nullptr,
                         google::protobuf::Message * response = nullptr);
      virtual ~BasicCall();

      /** waits for the call to finish */
      void wait();
      void timedWait(Time timeout);
      /** is the call finished ? */
      inline bool isFinished() const { return is_finished_; }
      /** has the call been canceled ? */
      inline bool isCanceled() const { return is_canceled_; }
      /** cancels the call */
      void cancel();

      inline void setServiceId(uint32_t id) { service_id_ = id; }
      inline void setMethodId(uint32_t id) { method_id_ = id; }
      inline void setTag(uint32_t id) { tag_ = id; }

      inline uint32_t serviceId() const { return service_id_; }
      inline uint32_t methodId() const { return method_id_; }
      inline uint32_t tag() const { return tag_; }

    protected:
      friend class Channel;

      /** we got the response ! */
      void finished();

      google::protobuf::Message * request_ = nullptr;
      google::protobuf::Message * response_ = nullptr;
      Mutex                       mutex_;
      Condition                   condition_;
      bool                        is_canceled_ = false;
      bool                        is_finished_ = false;
      uint32_t                    service_id_ = 0;
      uint32_t                    method_id_ = 0;
      uint32_t                    tag_ = 0;
    };
  }
}
