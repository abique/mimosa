#include <gflags/gflags.h>
#include <mimosa/init.hh>
#include <mimosa/rpc/server.hh>

#include "database.hh"

using namespace mimosa;

DEFINE_int32(port, 4242, "the port to use");

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  rpc::ServiceMap::Ptr service_map = new rpc::ServiceMap;
  service_map->add(new rpc::samples::Database);

  rpc::Server::Ptr server = new rpc::Server;
  server->setServiceMap(service_map);
  server->listenInet4(FLAGS_port);

  mimosa::deinit();
  return 0;
}
