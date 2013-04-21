#include <ctime>
#include <string>
#include <cstring>

#include "time.hh"

namespace mimosa
{
  namespace http
  {
    std::string time(::time_t t)
    {
      char buffer[64];

      ctime_r(&t, buffer); // needs at most 26 bytes
      buffer[::strlen(buffer) - 1] = '\0'; // remove trailing \n
      return buffer;
    }

    ::time_t time(const std::string & str)
    {
      tm tm;

      strptime(str.c_str(), "%a %b %d %H:%M:%S %Y", &tm);
      return mktime(&tm); // local time
    }
  }
}
