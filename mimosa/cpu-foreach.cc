/* Required for CPU_ALLOC(), ... */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <unistd.h>
#include <sched.h>

#include <cassert>
#include <memory>
#include <vector>

#include "cpu-count.hh"
#include "cpu-foreach.hh"
#include "thread.hh"

namespace mimosa
{
void cpuForeach(const std::function<void ()>& cb, bool affinity, size_t ratio)
{
  assert(ratio >= 1);

  std::vector<Thread> threads;
  size_t nproc = cpuCount();
  threads.resize(nproc * ratio);

  for (size_t i = 0; i < nproc * ratio; ++i)
  {
    threads[i].start([i, nproc, &cb, affinity, ratio] {
#ifdef HAS_SCHED_SETAFFINITY
      cpu_set_t * set = nullptr;
      if (affinity)
      {
        set = CPU_ALLOC(nproc);
        assert(set);
        CPU_ZERO_S(nproc, set);
        CPU_SET(i / ratio, set);

        sched_setaffinity(0, CPU_ALLOC_SIZE(nproc), set);
      }
#endif /* HAS_SCHED_SETAFFINITY */

      try {
        cb();
      } catch (...) {
      }
#ifdef HAS_SCHED_SETAFFINITY
      if (affinity)
        CPU_FREE(set);
#endif
    });
  }

  for (int i = 0; i < nproc * ratio; ++i)
    threads[i].join();
}
}
