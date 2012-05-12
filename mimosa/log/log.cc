#include <ctime>
#include <sstream>
#include <iomanip>

#include "../options/options.hh"
#include "../time.hh"
#include "../mutex.hh"
#include "log.hh"
#include "origin.hh"

namespace mimosa
{
  namespace log
  {
    static bool & enable_color = *options::addSwitch("log", "log-color", "enables colors in log");

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
      static Mutex stdout_lock;

      auto real_time = realTime();
      time_t ts = real_time / second;
      tm tm;
      ::localtime_r(&ts, &tm);
      std::ostringstream os;
      if (enable_color)
        os << log_colors[level];
      os << (tm.tm_year + 1900) << "-"
         << std::setfill('0') << std::setw(2) << (tm.tm_mon + 1) << "-"
         << std::setfill('0') << std::setw(2) << tm.tm_mday << " "
         << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
         << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
         << std::setfill('0') << std::setw(2) << tm.tm_sec << "."
         << std::setfill('0') << std::setw(4)
         << ((real_time % second) / millisecond) << " ";
      if (origin)
        os << origin->name_ << ": ";
      os << levelName(level) << ": "
         << msg << std::endl;
      if (enable_color)
        os << "\e[m";

      std::string str(os.str());
      Mutex::Locker locker(stdout_lock);
      ::write(STDOUT_FILENO, str.c_str(), str.size());
    }
  }
}
