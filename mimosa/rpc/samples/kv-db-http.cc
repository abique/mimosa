#include <mimosa/init.hh>
#include <mimosa/options/options.hh>
#include <mimosa/http/server.hh>

#include "database.hh"

using namespace mimosa;

uint16_t & PORT = *options::addOption<uint16_t>("", "port", "the port to use", 4242);

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  auto handler = new mimosa::rpc::samples::pb::DatabaseHttpHandler(new rpc::samples::Database, "/db/");

  http::Server::Ptr server(new http::Server);
  server->setReadTimeout(0);
  server->setWriteTimeout(0);
  server->setHandler(handler);
  if (!server->listenInet4(PORT)) {
    fprintf(stderr, "failed to bind on %d: %m\n", PORT);
    return 1;
  }
  printf("listen on %d succeed\n", PORT);

  while (true)
    server->serveOne(0, false);

  mimosa::deinit();
  return 0;
}
