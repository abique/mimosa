#include "../preproc.hh"

#ifdef MIMOSA_WIN
# include <winsock2.h>
# include <ws2tcpip.h>
#endif

#ifdef MIMOSA_UNIX
# include <poll.h>
# include <sys/socket.h>
#endif
