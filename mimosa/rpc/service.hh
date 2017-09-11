#pragma once

# include <string>

# include "../ref-countable.hh"
# include "basic-call.hh"
# include "protocol.hh"

namespace mimosa
{
  namespace rpc
  {
    class Channel;

    class Service : public RefCountable<Service>, private NonCopyable, private NonMovable
    {
    public:

      enum CallMethodStatus
      {
        kSucceed        = rpc::kSucceed,
        kMethodNotFound = rpc::kMethodNotFound,
        kNotImplemented = rpc::kNotImplemented,
        kInvalidMsg     = rpc::kInvalidMsg,
      };

      virtual ~Service() {}

      virtual uint32_t id() const = 0;
      virtual const char * name() const = 0;

    protected:
      friend class Channel;

      virtual CallMethodStatus callMethod(BasicCall::Ptr call,
                                          const char *   request_data,
                                          uint32_t       request_size);
    };
  }
}

