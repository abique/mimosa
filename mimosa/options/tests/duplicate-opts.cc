/*
 * This test is meant to check that having two time the same option
 * will generate a failure.
 */

#include "../options.hh"

bool * enable_x = mimosa::options::addSwitch("group", "enable-x", "enables feature x");
bool * enable_y = mimosa::options::addSwitch("group", "enable-x", "enables feature x");

int main(int argc, char ** argv)
{
  mimosa::options::parse(argc, argv);

  return 0;
}
