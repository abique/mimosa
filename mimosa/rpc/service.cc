#include <cassert>

#include "service.hh"

namespace mimosa
{
  namespace rpc
  {
    Service::CallMethodStatus
    Service::callMethod(BasicCall::Ptr /*call*/,
                        const char *   /*request_data*/,
                        uint32_t       /*request_size*/)
    {
      assert(false && "should not get called!");
      return kNotImplemented;
    }
  }
}
