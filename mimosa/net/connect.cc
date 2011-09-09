#include <netdb.h>
#include <sys/socket.h>
#include <cstring>
#include <melon/melon.h>

#include "connect.hh"

namespace mimosa
{
  namespace net
  {
    int connectToHost(const std::string &host, uint16_t port,
                      runtime::Time timeout)
    {
      ::hostent * host_entry = gethostbyname(host.c_str());
      if (!host_entry)
        return -1;

      int fd = ::socket(host_entry->h_addrtype, SOCK_STREAM, 0);
      if (fd < 0)
        return -1;

      if (host_entry->h_addrtype == AF_INET)
      {
        sockaddr_in addr;
        ::memset(&addr, 0, sizeof (addr));
        addr.sin_family = AF_INET;
        addr.sin_port   = ::htons(port);
        ::memcpy(&addr.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);
        if (::melon_connect(fd, (const sockaddr*)&addr, sizeof (addr), timeout))
        {
          ::close(fd);
          return -1;
        }
        return fd;
      }
      if (host_entry->h_addrtype == AF_INET6)
      {
        sockaddr_in6 addr;
        ::memset(&addr, 0, sizeof (addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port   = ::htons(port);
        ::memcpy(&addr.sin6_addr, host_entry->h_addr_list[0], host_entry->h_length);
        if (::melon_connect(fd, (const sockaddr*)&addr, sizeof (addr), timeout))
        {
          ::close(fd);
          return -1;
        }
        return fd;
      }
      ::close(fd);
      return -1;
    }
  }
}
