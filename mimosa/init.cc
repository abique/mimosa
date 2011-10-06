#include <signal.h>
#include <cassert>
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

    google::ParseCommandLineFlags(&argc, &argv, true);
    ::gnutls_global_init();
  }

  void deinit()
  {
    ::gnutls_global_deinit ();
    google::ShutDownCommandLineFlags();
  }
}
