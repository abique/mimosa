#include <sstream>

#include <mimosa/init.hh>
#include <mimosa/cpu-foreach.hh>
#include <mimosa/options/options.hh>
#include <mimosa/http/server.hh>
#include <mimosa/http/dispatch-handler.hh>
#include <mimosa/http/fs-handler.hh>
#include <mimosa/http/log-handler.hh>
#include <mimosa/uri/parse-query.hh>

using namespace mimosa;

uint16_t & PORT = *options::addOption<uint16_t>("", "port", "the port to use", 4242);
std::string & PATH = *options::addOption<std::string>("", "path", "the data dir to expose", "/usr");
std::string & CERT = *options::addOption<std::string>("", "cert", "the certificate (cert.pem)", "");
std::string & KEY = *options::addOption<std::string>("", "key", "the key (key.pem)", "");
uint64_t & TIMEOUT = *options::addOption<uint64_t>("", "timeout", "the read & write timeout in ms", 5000);

class HelloHandler : public mimosa::http::Handler
{
  bool handle(mimosa::http::RequestReader &  /*request*/,
                      mimosa::http::ResponseWriter & response) const override
  {
    std::ostringstream os;
    os <<
      "<html><head><title>Hello World!</title></head>"
      "<body>Hello World!<hr/>"
      "<ul>"
      "<li>browse <a href='data/'>" << PATH << "</a></li>"
      "<li>test <a href='query-echo?a=b&c=d'>query string</a></li>"
      "<li>test <a href='post-echo'>post data</a></li>"
      "</ul>"
      "</body></html>";

    std::string data(os.str());
    response.setStatus(mimosa::http::kStatusOk);
    response.setContentType("text/html");
    response.write(data.data(), data.size());
    return true;
  }
};

class QueryEchoHandler : public mimosa::http::Handler
{
  bool handle(mimosa::http::RequestReader &  request,
                      mimosa::http::ResponseWriter & response) const override
  {
    std::ostringstream os;
    os <<
      "<html><head><title>Query Echo!</title></head>"
      "<body>Query Echo!<hr/>"
      "<ul>";
    for (const auto & it : request.query())
      os << "<li>" << it.first << " = " << it.second << "</li>";
    os <<
      "</ul>"
      "</body></html>";

    std::string data(os.str());
    response.setStatus(mimosa::http::kStatusOk);
    response.setContentType("text/html");
    response.write(data.data(), data.size());
    return true;
  }
};

class PostEchoHandler : public mimosa::http::Handler
{
  bool handle(mimosa::http::RequestReader &  request,
                      mimosa::http::ResponseWriter & response) const override
  {
    stream::Buffer buffer(request.contentLength());
    int64_t rbytes = request.loopRead(buffer.data(), request.contentLength());
    if (rbytes < request.contentLength())
      return false;
    kvs form;
    uri::parseQuery(buffer.data(), buffer.size(), &form);

    std::ostringstream os;
    os <<
      "<html><head><title>Post Echo!</title></head>"
      "<body>Post Echo!<hr/>"
      "Method: " << methodString(request.method()) << "<br/>"
      "Content-Type: " << request.contentType() << "<br/>"
      "<ul>";
    for (const auto & it : form)
      os << "<li>" << it.first << " = " << it.second << "</li>";
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
    response.setStatus(mimosa::http::kStatusOk);
    response.setContentType("text/html");
    response.write(data.data(), data.size());
    return true;
  }
};

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  {
    auto dispatch(new http::DispatchHandler);
    dispatch->registerHandler("/", new HelloHandler);
    dispatch->registerHandler("/query-echo", new QueryEchoHandler);
    dispatch->registerHandler("/post-echo", new PostEchoHandler);
    auto fs_handler = new http::FsHandler(PATH, 1);
    fs_handler->enableReaddir(true);
    dispatch->registerHandler("/data/*", fs_handler);

    auto logger(new http::LogHandler);
    logger->setHandler(dispatch);

    http::Server::Ptr server(new http::Server);
    server->setReadTimeout(TIMEOUT * millisecond);
    server->setWriteTimeout(TIMEOUT * millisecond);
    server->setHandler(logger);

    if (!CERT.empty() && !KEY.empty())
      server->setSecure(CERT, KEY);

    auto ret = server->listenInet4(PORT);
    if (!ret)
    {
      puts("failed to listen");
      return 1;
    }
    printf("listen on %d succeed\n", PORT);

    cpuForeach([server] {
        while (true)
          server->serveOne();
      });
  }

  mimosa::deinit();
  return 0;
}
