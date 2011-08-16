#include <mimosa/main.hh>
#include <mimosa/http/server.hh>

using namespace mimosa;

http::Server::Ptr g_server;

MIMOSA_MAIN(argc, argv)
{
  (void)argc;
  (void)argv;

  g_server = new http::Server;
  auto ret = g_server->listenInet4(4242);
  if (!ret)
    puts("failed to listen");
  //assert(false);
  return 0;
}
