#ifndef MIMOSA_RPC_SERVER_HH
# define MIMOSA_RPC_SERVER_HH

# include "../non-copyable.hh"
# include "../net/server.hh"
# include "service-map.hh"

namespace mimosa
{
  namespace rpc
  {
    class Server : public net::Server,
        private NonCopyable
    {
    public:
      MIMOSA_DEF_PTR(Server);

      Server();

      inline ServiceMap::ConstPtr serviceMap() const { return service_map_; }
      inline void setServiceMap(ServiceMap::ConstPtr service_map)
      { service_map_ = service_map; }

      static void serve(Server::Ptr server, int fd);

    protected:
      ServiceMap::ConstPtr service_map_;
    };
  }
}

#endif // !MIMOSA_RPC_SERVER_HH
