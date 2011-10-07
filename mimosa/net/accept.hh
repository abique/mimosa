#ifndef MIMOSA_NET_ACCEPT_HH
# define MIMOSA_NET_ACCEPT_HH

# include <sys/socket.h>

# include "../runtime/time.hh"

namespace mimosa
{
  namespace net
  {
    int accept(int socket, ::sockaddr * address, socklen_t * address_len,
               runtime::Time timeout);
  }
}

#endif /* !MIMOSA_NET_ACCEPT_HH */
