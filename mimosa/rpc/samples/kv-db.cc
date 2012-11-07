#include <mimosa/init.hh>
#include <mimosa/options/options.hh>
#include <mimosa/rpc/server.hh>

#include "database.hh"

using namespace mimosa;

uint16_t & PORT = *options::addOption<uint16_t>("", "port", "the port to use", 4242);

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  rpc::ServiceMap::Ptr service_map = new rpc::ServiceMap;
  service_map->add(new rpc::samples::Database);

  rpc::Server::Ptr server = new rpc::Server;
  server->setServiceMap(service_map.get());
  server->listenInet4(PORT);

  while (true)
    server->serveOne();

  mimosa::deinit();
  return 0;
}
