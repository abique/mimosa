#include <ctime>
#include <string>

#include "time.hh"

namespace mimosa
{
  namespace http
  {
    std::string time(::time_t t)
    {
      char buffer[64];
      tm   tm;

      asctime_r(gmtime_r(&t, &tm), buffer);
      return buffer;
    }
  }
}
