#include <mimosa/init.hh>
#include <mimosa/options/options.hh>

#include "../client-channel.hh"

std::string & URL = *mimosa::options::addOption<std::string>("", "url", "the webpage to download", "");
std::string & OUT = *mimosa::options::addOption<std::string>("", "out", "where to save?", "");

int main(int argc, char **argv)
{
  mimosa::init(argc, argv);

  mimosa::http::ClientChannel  cc;
  mimosa::http::RequestWriter  rq(cc);
  mimosa::http::ResponseReader rp(cc);

  mimosa::deinit();
  return 0;
}
