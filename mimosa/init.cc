#include <signal.h>
#include <cassert>
#include <cerrno>
#include <gnutls/gnutls.h>
#include <pthread.h>
#include <gflags/gflags.h>

#include "init.hh"
#include "runtime/fiber.hh"

namespace mimosa
{
  static void dummy(int)
  {
  }

  void init(int argc, char ** argv)
  {
    ::signal(SIGPIPE, dummy);

    google::ParseCommandLineFlags(&argc, &argv, true);
    ::gnutls_global_set_mutex(gnutls_mutex_init, gnutls_mutex_deinit,
                              gnutls_mutex_lock, gnutls_mutex_unlock);
    ::gnutls_global_init();
  }

  void deinit()
  {
    ::gnutls_global_deinit ();
    google::ShutDownCommandLineFlags();
  }
}
