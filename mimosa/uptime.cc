#include "uptime.hh"

namespace mimosa
{
  Time uptime() noexcept
  {
    static const Time start = monotonicTimeCoarse();
    return monotonicTimeCoarse() - start;
  }
}
