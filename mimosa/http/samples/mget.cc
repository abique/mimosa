#include <mimosa/init.hh>
#include <mimosa/options/options.hh>

#include "../client-channel.hh"
#include "../../stream/fd-stream.hh"
#include "../../stream/copy.hh"

std::string & URL = *mimosa::options::addOption<std::string>("", "url", "the webpage to download", "");
std::string & OUT = *mimosa::options::addOption<std::string>("", "out", "where to save?", "");

int main(int argc, char **argv)
{
  mimosa::init(argc, argv);

  mimosa::http::ClientChannel cc;
  auto response = cc.get(URL);
  if (!response)
    return 1;

  auto file = mimosa::stream::FdStream::openFile(OUT.c_str(), O_RDWR | O_CREAT | O_TRUNC);
  if (!file)
    return 1;

  mimosa::stream::copy(*response, *file);

  mimosa::deinit();
  return 0;
}
