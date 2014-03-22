#include <mimosa/init.hh>
#include <mimosa/options/options.hh>

#include "../client-channel.hh"
#include "../../flat/flat.hh"
#include "../../stream/copy.hh"

std::string & URL = *mimosa::options::addOption<std::string>("", "url", "the webpage to download", "");
std::string & IN = *mimosa::options::addOption<std::string>("", "in", "file to upload", "");

int main(int argc, char **argv)
{
  mimosa::init(argc, argv);

  mimosa::flat::Flat flat(IN, O_RDONLY, 0666);
  if (!flat.ok())
    return 1;

  mimosa::http::ClientChannel cc;
  mimosa::http::RequestWriter rw(cc);
  rw.setUrl(URL);
  rw.setMethod(mimosa::http::kMethodPut);
  rw.setProto(1, 1);
  rw.setContentLength(flat.size());
  rw.setContentType("image/png");
  if (!rw.sendRequest())
    return 2;
  rw.loopWrite((const char *)flat.base(), flat.size());
  auto response = rw.response();
  if (!response)
    return 3;

  mimosa::deinit();
  return 0;
}
