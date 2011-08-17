#include <gflags/gflags.h>

#include <mimosa/main.hh>
#include <mimosa/http/server.hh>

using namespace mimosa;

DEFINE_int32(port, 4242, "the port to use");
DEFINE_string(path, "/usr/include", "the data dir to expose");

class HelloHandler : public mimosa::http::Handler
{
  virtual bool handle(mimosa::http::Request &        request,
                      mimosa::http::ResponseWriter & response) const
  {
    static const std::string hello_world =
      "<html><head><title>Hello World!</title></head>"
      "<body>Hello World!</body></html>";

    response.status_ = mimosa::http::kStatusOk;
    response.content_type_ = "text/html";
    response.write(hello_world.data(), hello_world.size());
    return true;
  }
};

MIMOSA_MAIN(argc, argv)
{
  (void)argc;
  (void)argv;

  http::Server::Ptr server = new http::Server;
  server->setHandler(new HelloHandler);
  auto ret = server->listenInet4(FLAGS_port);
  if (!ret)
  {
    puts("failed to listen");
    return 1;
  }

  printf("listen on %d succeed\n", FLAGS_port);
  mimosa::runtime::sleep(mimosa::runtime::minutes(15));
  server->stop();
  return 0;
}
