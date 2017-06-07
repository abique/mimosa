#pragma once

# include <map>

# include "../ref-countable.hh"
# include "../shared-mutex.hh"
# include "service.hh"

namespace mimosa
{
  namespace rpc
  {
    class ServiceMap : public RefCountable<ServiceMap>
    {
    public:
      ~ServiceMap();

      Service::Ptr find(uint32_t service_id) const;

      void add(Service::Ptr service);
      void remove(Service::Ptr service);

    private:
      mutable SharedMutex              lock_;
      std::map<uint32_t, Service::Ptr> services_;
    };
  }
}

