#include <mimosa/main.hh>
#include <mimosa/http/server.hh>

using namespace mimosa;

http::Server::Ptr g_server;

MIMOSA_MAIN(argc, argv)
{
  (void)argc;
  (void)argv;

  g_server = new http::Server;
  assert(false);
  auto ret = g_server->listenInet4(4242);
  return 0;
}
