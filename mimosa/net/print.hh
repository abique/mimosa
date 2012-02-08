#ifndef MIMOSA_NET_PRINT_HH
# define MIMOSA_NET_PRINT_HH

# include <sys/types.h>
# include <sys/socket.h>

# include <string>

namespace mimosa
{
  namespace net
  {
    std::string print(const struct ::sockaddr *addr, ::socklen_t len);
  }
}

#endif /* !MIMOSA_NET_PRINT_HH */
