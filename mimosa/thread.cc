#include <pthread.h>
#include <climits>
#include <cassert>
#include <memory>

#include <string>

#include "thread.hh"

namespace mimosa
{
  void setCurrentThreadName(const char *name) noexcept
  {
#ifdef __MACH__
    pthread_setname_np(name.c_str());
#elif defined(__WIN32__)
#elif defined(__HAIKU__)
#else
    pthread_setname_np(pthread_self(), name);
#endif
  }
}
