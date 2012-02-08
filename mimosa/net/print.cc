#include <netdb.h>

#include "print.hh"

namespace mimosa
{
  namespace net
  {
    std::string print(const struct ::sockaddr *addr, ::socklen_t len)
    {
      char host[80] = { '\0' };

      getnameinfo(addr, len, host, sizeof (host), nullptr, 0, NI_NUMERICHOST);
      return host;
    }
  }
}
