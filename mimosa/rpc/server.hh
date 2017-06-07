#pragma once

# include "../net/server.hh"
# include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    class Server : public net::Server
    {
    public:
      MIMOSA_DEF_PTR(Server);

      Server();

      inline ServiceMap::ConstPtr serviceMap() const { return service_map_; }
      inline void setServiceMap(ServiceMap::ConstPtr service_map)
      { service_map_ = service_map; }

      virtual void serve(int                fd,
                         const ::sockaddr * address,
                         socklen_t          address_len) const;

    protected:
      ServiceMap::ConstPtr service_map_;
    };
  }
}

