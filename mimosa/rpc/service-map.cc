#include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    ServiceMap::~ServiceMap()
    {
      sync::RWLock::Locker locker(rwlock_);
    }

    Service::Ptr
    ServiceMap::find(uint32_t service_id) const
    {
      sync::RWLock::ReadLocker locker(rwlock_);
      auto it = services_.find(service_id);
      if (it == services_.end())
        return nullptr;
      return it->second;
    }

    void
    ServiceMap::add(Service::Ptr service)
    {
      sync::RWLock::Locker locker(rwlock_);
      services_[service->id()] = service;
    }

    void
    ServiceMap::remove(Service::Ptr service)
    {
      sync::RWLock::Locker locker(rwlock_);
      services_.erase(service->id());
    }
  }
}
