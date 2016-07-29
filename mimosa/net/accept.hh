#pragma once

#

#include <sys/socket.h>

#include "../time.hh"

namespace mimosa
{
  namespace net
  {
    int accept(int socket, ::sockaddr * address, socklen_t * address_len,
               Time timeout);
  }
}
