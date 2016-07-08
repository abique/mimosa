#ifdef __unix__
# include <poll.h>
# include <netdb.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <cstring>
#include <cerrno>
#include <memory>

#include "io.hh"
#include "connect.hh"

namespace mimosa
{
  namespace net
  {
    int connect(int socket, const struct sockaddr *address,
                socklen_t address_len, Time timeout)
    {
      if (!waitForFdReady(socket, POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI, timeout))
        return -1;
      return ::connect(socket, address, address_len);
    }

    int connectToHost(const StringRef &host, uint16_t port, Time timeout)
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

    int connectToUnixSocket(const std::string & path,
                            Time timeout)
    {
      size_t addr_len = ((size_t) (((::sockaddr_un *) 0)->sun_path)) + path.size() + 1;
      std::unique_ptr<char[]> data(new char[addr_len]);
      ::sockaddr_un & addr = *(::sockaddr_un*)data.get();
      addr.sun_family = AF_UNIX;
      ::memcpy(addr.sun_path, path.c_str(), path.size() + 1);

      int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
      if (fd < 0)
        return -1;

      if (connect(fd, (const sockaddr*)&addr, addr_len, timeout))
      {
        ::close(fd);
        return -1;
      }
      return fd;
    }
  }
}
