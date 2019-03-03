#pragma once

#include <sched.h>

namespace mimosa
{
  inline void yield() noexcept
  {
     ::sched_yield();
  }
}
