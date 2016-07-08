#if __WIN32__
# include <windows.h>
#endif

#include <unistd.h>

#include "cpu-count.hh"

namespace mimosa
{
  int cpuCount()
  {
#ifdef __WIN32__
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
  }
}
