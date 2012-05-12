#include "io.hh"
#include "accept.hh"

namespace mimosa
{
  namespace net
  {
    int accept(int socket, struct sockaddr * address, socklen_t * address_len,
               Time timeout)
    {
      if (!waitForFdReady(socket, POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI, timeout))
        return -1;
      return ::accept(socket, address, address_len);
    }
  }
}
