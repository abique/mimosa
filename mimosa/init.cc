#include <sched.h>
#include <signal.h>
#include <cassert>
#include <cstring>
#include <cerrno>

#include <unistd.h>

#include <gnutls/gnutls.h>

#include "init.hh"
#include "options/options.hh"

namespace mimosa
{
  static void dummy(int)
  {
  }

  static void gnutls_log(int /*level*/, const char *msg)
  {
    printf("%s", msg);
  }

  void init(int argc, char ** argv)
  {
    ::signal(SIGPIPE, dummy);
    ::sched_param sc_params;
    sc_params.sched_priority = 1;
    if (::sched_setscheduler(::getpid(), SCHED_FIFO, &sc_params))
      ::fprintf(stderr, "sched_setscheduler(): %s\n", ::strerror(errno));
    options::parse(argc, argv);
    ::gnutls_global_init();
    ::gnutls_global_set_log_level(0);
    ::gnutls_global_set_log_function(gnutls_log);
  }

  void deinit()
  {
    ::gnutls_global_deinit ();
    options::deinit();
  }
}
