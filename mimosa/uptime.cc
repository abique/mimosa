#include "uptime.hh"

namespace mimosa
{
  Time uptime()
  {
    static const Time start = monotonicTimeCoarse();
    return monotonicTimeCoarse() - start;
  }
}
