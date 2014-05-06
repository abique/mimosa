#include <unistd.h>

#include "cpu-count.hh"

namespace mimosa
{
  int cpuCount()
  {
    return sysconf(_SC_NPROCESSORS_ONLN);
  }
}
