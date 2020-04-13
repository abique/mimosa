#pragma once

#include <functional>

namespace mimosa
{
  /**
   * Will create one thread for each cpu, set the cpu affinity
   * and execute cb.
   *
   * Thread created in cb will by default keep the same cpu affinity.
   */
  template <typename Callback>
  void cpuForeach(const Callback& cb, bool affinity = false, size_t ratio = 1);
}

#include "cpu-foreach.hxx"