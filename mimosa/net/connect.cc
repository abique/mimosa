#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#include "io.hh"
#include "connect.hh"

namespace mimosa
{
  namespace net
  {
    int connect(int socket, const struct sockaddr *address,
                socklen_t address_len, runtime::Time timeout)
    {
      if (!waitForFdReady(socket, POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI, timeout))
        return -1;
      return ::connect(socket, address, address_len);
    }

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
        addr.sin_port   = htons(port);
        ::memcpy(&addr.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);
        if (connect(fd, (const sockaddr*)&addr, sizeof (addr), timeout))
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
        addr.sin6_port   = htons(port);
        ::memcpy(&addr.sin6_addr, host_entry->h_addr_list[0], host_entry->h_length);
        if (connect(fd, (const sockaddr*)&addr, sizeof (addr), timeout))
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
