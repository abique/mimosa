#include <mimosa/main.hh>
#include <mimosa/rpc/server.hh>

#include "database.hh"

using namespace mimosa;

MIMOSA_MAIN(argc, argv)
{
  (void)argc;
  (void)argv;

  rpc::ServiceMap::Ptr service_map = new rpc::ServiceMap;
  service_map->add(new rpc::samples::Database);

  rpc::Server::Ptr server = new rpc::Server;
  server->setServiceMap(service_map);
  server->listenInet4(4242);
  return 0;
}
