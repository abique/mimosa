#include <sstream>

#include <gflags/gflags.h>

#include <mimosa/main.hh>
#include <mimosa/http/server.hh>
#include <mimosa/http/dispatch-handler.hh>
#include <mimosa/http/fs-handler.hh>

using namespace mimosa;

DEFINE_int32(port, 4242, "the port to use");
DEFINE_string(path, "/usr/include", "the data dir to expose");
DEFINE_string(cert, "", "the certificate (cert.pem)");
DEFINE_string(key, "", "the key (key.pem)");

class HelloHandler : public mimosa::http::Handler
{
  virtual bool handle(mimosa::http::RequestReader &  /*request*/,
                      mimosa::http::ResponseWriter & response) const
  {
    std::ostringstream os;
    os <<
      "<html><head><title>Hello World!</title></head>"
      "<body>Hello World!<hr/>"
      "<ul>"
      "<li>browse <a href='data/'>" << FLAGS_path << "</a></li>"
      "<li>test <a href='query-echo?a=b&c=d'>query string</a></li>"
      "<li>test <a href='post-echo'>post data</a></li>"
      "</ul>"
      "</body></html>";

    std::string data(os.str());
    response.status_ = mimosa::http::kStatusOk;
    response.content_type_ = "text/html";
    response.write(data.data(), data.size());
    return true;
  }
};

class QueryEchoHandler : public mimosa::http::Handler
{
  virtual bool handle(mimosa::http::RequestReader &  request,
                      mimosa::http::ResponseWriter & response) const
  {
    std::ostringstream os;
    os <<
      "<html><head><title>Query Echo!</title></head>"
      "<body>Query Echo!<hr/>"
      "<ul>";
    for (auto it = request.query().cbegin(); it != request.query().cend(); ++it)
      os << "<li>" << it->first << " = " << it->second << "</li>";
    os <<
      "</ul>"
      "</body></html>";

    std::string data(os.str());
    response.status_ = mimosa::http::kStatusOk;
    response.content_type_ = "text/html";
    response.write(data.data(), data.size());
    return true;
  }
};

class PostEchoHandler : public mimosa::http::Handler
{
  virtual bool handle(mimosa::http::RequestReader &  request,
                      mimosa::http::ResponseWriter & response) const
  {
    std::ostringstream os;
    os <<
      "<html><head><title>Post Echo!</title></head>"
      "<body>Post Echo!<hr/>"
      "Method: " << methodString(request.method()) << "<br/>"
      "Content-Type: " << request.contentType() << "<br/>"
      "<ul>";
    container::kvs form = request.form();
    for (auto it = form.cbegin(); it != form.cend(); ++it)
      os << "<li>" << it->first << " = " << it->second << "</li>";
    os <<
      "</ul>"
      "<hr/>"
      "<form method='post' enctype='application/x-www-form-urlencoded' name='test-form'>"
      "firstname <input type='text' name='fname'/><br/>"
      "lastname <input type='text' name='lname'/><br/>"
      "<input type='submit' value='submit'/>"
      "</form>"
      "</body></html>";

    std::string data(os.str());
    response.status_ = mimosa::http::kStatusOk;
    response.content_type_ = "text/html";
    response.write(data.data(), data.size());
    return true;
  }
};

MIMOSA_MAIN(argc, argv)
{
  (void)argc;
  (void)argv;

  auto dispatch(new http::DispatchHandler);
  dispatch->registerHandler("/", new HelloHandler);
  dispatch->registerHandler("/query-echo", new QueryEchoHandler);
  dispatch->registerHandler("/post-echo", new PostEchoHandler);
  dispatch->registerHandler("/data/*", new http::FsHandler(FLAGS_path, 1, true));
  http::Server::Ptr server = new http::Server;
  server->setHandler(dispatch);
  if (!FLAGS_cert.empty() && !FLAGS_key.empty())
    server->setSecure(FLAGS_cert, FLAGS_key);
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
