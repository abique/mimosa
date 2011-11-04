#include <ctime>
#include <sstream>

#include "../options/options.hh"
#include "../runtime/time.hh"
#include "../sync/mutex.hh"
#include "log.hh"

namespace mimosa
{
  namespace log
  {
    static bool * enable_color = options::addSwitch("log", "log-color", "enables colors in log");

    static const char * const log_colors[] = {
      "\e[0;32m", // debug
      "\e[m",     // info
      "\e[0;33m", // warning
      "\e[0;31m", // error
      "\e[1;31m", // critical
      "\e[1;35m", // fatal
    };

    void log(Level level, const Origin * origin, const std::string & msg)
    {
      static sync::Mutex stdout_lock;

      auto real_time = runtime::realTime();
      time_t ts = real_time / runtime::second;
      tm tm;
      ::localtime_r(&ts, &tm);
      std::ostringstream os;
      if (*enable_color)
        os << log_colors[level];
      os << (tm.tm_year + 1900) << "-" << (tm.tm_mon + 1) << "-"
         << tm.tm_mday << " " << tm.tm_hour << ":" << tm.tm_min
         << ":" << tm.tm_sec << "."
         << ((real_time % runtime::second) / runtime::millisecond)
         << " ";
      if (origin)
        os << origin->name_ << ": ";
      os << levelName(level) << ": "
         << msg << std::endl;

      std::string str(os.str());
      sync::Mutex::Locker locker(stdout_lock);
      ::write(STDOUT_FILENO, str.c_str(), str.size());
    }
  }
}
