#pragma once

#include <sys/types.h>

#if defined(__unix__) || defined(__APPLE__) || defined(__HAIKU__)
# include <sys/socket.h>
# include <netdb.h>
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

