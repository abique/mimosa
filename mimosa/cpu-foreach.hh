#pragma once

# include <functional>

namespace mimosa
{
  /**
   * Will create one thread for each cpu, set the cpu affinity
   * and execute cb.
   *
   * Thread created in cb will by default keep the same cpu affinity.
   */
  void cpuForeach(const std::function<void ()>& cb, bool affinity = false, size_t ratio = 1);
}

