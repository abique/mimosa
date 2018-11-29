#include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    ServiceMap::~ServiceMap()
    {
      SharedMutex::Locker locker(lock_);
    }

    Service::Ptr
    ServiceMap::find(uint32_t service_id) const
    {
      SharedMutex::ReadLocker locker(lock_);
      auto it = services_.find(service_id);
      if (it == services_.end())
        return nullptr;
      return it->second;
    }

    void
    ServiceMap::add(const Service::Ptr& service)
    {
      SharedMutex::Locker locker(lock_);
      services_[service->id()] = service;
    }

    void
    ServiceMap::remove(const Service::Ptr& service)
    {
      SharedMutex::Locker locker(lock_);
      services_.erase(service->id());
    }
  }
}
