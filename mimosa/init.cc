#include <sched.h>
#include <signal.h>
#include <cassert>
#include <cstring>
#include <cerrno>

#include <gnutls/gnutls.h>
#include <gflags/gflags.h>

#include "init.hh"

namespace mimosa
{
  static void dummy(int)
  {
  }

  void init(int argc, char ** argv)
  {
    ::signal(SIGPIPE, dummy);
    ::sched_param sc_params;
    sc_params.sched_priority = 1;
    if (::sched_setscheduler(::getpid(), SCHED_FIFO, &sc_params))
      ::fprintf(stderr, "sched_setscheduler(): %s\n", ::strerror(errno));
    google::ParseCommandLineFlags(&argc, &argv, true);
    ::gnutls_global_init();
  }

  void deinit()
  {
    ::gnutls_global_deinit ();
    google::ShutDownCommandLineFlags();
  }
}
