#ifndef MIMOSA_CPU_FOREACH_HH
# define MIMOSA_CPU_FOREACH_HH

# include <functional>

namespace mimosa
{
  /**
   * Will create one thread for each cpu, set the cpu affinity
   * and execute cb.
   *
   * Thread created in cb will by default keep the same cpu affinity.
   */
  void cpuForeach(const std::function<void ()>& cb);
}

#endif /* !MIMOSA_CPU_FOREACH_HH */
