#include "../log.hh"
#include "../../init.hh"

auto * network = new mimosa::log::Origin("network", mimosa::log::Info);

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  MIMOSA_LOG(Debug, network, "debug message");
  MIMOSA_LOG(Info, network, "info message");
  MIMOSA_LOG(Warning, network, "warning message");
  MIMOSA_LOG(Error, network, "error message");
  MIMOSA_LOG(Critical, network, "critical message");
  MIMOSA_LOG(Fatal, network, "fatal message %d", 1);

  mimosa::deinit();
  return 0;
}
