#include <ctime>
#include <sstream>

#include "../runtime/time.hh"
#include "../sync/mutex.hh"
#include "log.hh"

static const char * const level_prefix[] = {
  "debug",
  "info",
  "warning",
  "error",
  "critical",
  "fatal",
  nullptr
};

static mimosa::sync::Mutex stdout_lock;

namespace mimosa
{
  namespace log
  {
    void log(Level level, const Origin & origin, const std::string & msg)
    {
      auto real_time = runtime::realTime();
      time_t ts = real_time / runtime::second;
      tm tm;
      ::localtime_r(&ts, &tm);
      std::ostringstream os;
      os << (tm.tm_year + 1900) << "-" << (tm.tm_mon + 1) << "-"
         << tm.tm_mday << " " << tm.tm_hour << ":" << tm.tm_min
         << ":" << tm.tm_sec << "."
         << ((real_time % runtime::second) / runtime::millisecond)
         << " " << origin.name_ << ": " << level_prefix[level] << ": "
         << msg << std::endl;

      std::string str(os.str());
      sync::Mutex::Locker locker(stdout_lock);
      ::write(STDOUT_FILENO, str.c_str(), str.size());
    }
  }
}
