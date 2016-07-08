#pragma once

#include <sys/types.h>

#ifdef __unix__
# include <sys/socket.h>
#endif

#ifdef __WIN32__
# include <winsock2.h>
#endif

#include <string>

namespace mimosa
{
  namespace net
  {
    std::string print(const struct ::sockaddr *addr, ::socklen_t len);
  }
}

