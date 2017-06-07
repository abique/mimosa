#pragma once

#include <sched.h>

namespace mimosa
{
  inline void yield() { ::sched_yield(); }
}
