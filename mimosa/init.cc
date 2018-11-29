#include <sched.h>
#include <csignal>
#include <cassert>
#include <cstring>
#include <cerrno>

#include <unistd.h>

#include <gnutls/gnutls.h>

#include "init.hh"
#include "options/options.hh"
#include "uptime.hh"
#include "log/log.hh"

namespace mimosa
{
  static void gnutls_log(int /*level*/, const char *msg)
  {
    log::info("[gnutls] %s", msg);
  }

  void init(int argc, char ** argv)
  {
    // initialize uptime
    uptime();

    ::signal(SIGPIPE, SIG_IGN);
    options::parse(argc, argv);
    ::gnutls_global_init();
    ::gnutls_check_version(GNUTLS_VERSION);
    ::gnutls_global_set_log_level(0);
    ::gnutls_global_set_log_function(gnutls_log);
  }

  void deinit()
  {
    ::gnutls_global_deinit ();
    options::deinit();
  }
}
