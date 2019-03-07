#if __WIN32__
# include <windows.h>
#endif

#include <unistd.h>

#include "cpu-count.hh"

namespace mimosa
{
  size_t cpuCount() noexcept
  {
#ifdef __WIN32__
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
#else
    return static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
#endif
  }
}
