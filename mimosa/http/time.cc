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
      // remove trailing \n
      buffer[::strlen(buffer) - 1] = '\0';
      return buffer;
    }
  }
}
