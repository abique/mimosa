#ifndef MIMOSA_RPC_SERVICE_HH
# define MIMOSA_RPC_SERVICE_HH

# include <string>

# include "../ref-countable.hh"
# include "basic-call.hh"

namespace mimosa
{
  namespace rpc
  {
    class Service : public RefCountable<Service>
    {
    public:

      enum CallMethodStatus
      {
        kSucceed            = 0,
        kMethodNotFound     = 1,
        kNotImplemented     = 2,
        kInvalidRequestData = 3,
      };

      virtual ~Service() {}

      virtual uint32_t id() const = 0;
      virtual const char * name() const = 0;

    protected:
      virtual CallMethodStatus callMethod(BasicCall::Ptr call,
                                          const char *   request_data,
                                          uint32_t       request_size);
    };
  }
}

#endif /* !MIMOSA_RPC_SERVICE_HH */
