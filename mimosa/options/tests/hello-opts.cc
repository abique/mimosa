#include <iostream>

#include "../options.hh"

bool * enable_x = mimosa::options::addSwitch("group", "enable-x", "enables feature x");
std::string * toto = mimosa::options::addOption<std::string>(
  "group2", "toto", "toto's name", "(unamed toto)");

int main(int argc, char ** argv)
{
  mimosa::options::parse(argc, argv);

  std::cout << "feature x " << (*enable_x ? "enabled" : "disabled") << std::endl;
  std::cout << "toto's name: " << *toto << std::endl;

  return 0;
}
