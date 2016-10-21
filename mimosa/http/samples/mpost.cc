#include <mimosa/init.hh>
#include <mimosa/options/options.hh>
#include <mimosa/stream/direct-fd-stream.hh>

#include "../client-channel.hh"
#include "../../stream/copy.hh"

std::string & URL = *mimosa::options::addOption<std::string>("", "url", "the webpage to download", "");
std::string & DATA = *mimosa::options::addOption<std::string>("", "data", "data to upload", "");

int main(int argc, char **argv)
{
  mimosa::init(argc, argv);

  mimosa::http::ClientChannel cc;
  mimosa::http::RequestWriter rw(cc);
  rw.setUrl(URL);
  rw.setMethod(mimosa::http::kMethodPost);
  rw.setProto(1, 1);
  rw.setContentLength(DATA.size());
  rw.setContentType("application/json");
  if (!rw.sendRequest())
    return 2;
  rw.loopWrite((const char *)DATA.data(), DATA.size());
  auto response = rw.response();
  if (!response)
    return 3;

  mimosa::stream::DirectFdStream out(STDOUT_FILENO, false);
  mimosa::stream::copy(*response, out);

  mimosa::deinit();
  return 0;
}
