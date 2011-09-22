#include <signal.h>
#include <cassert>
#include <cerrno>
#include <gnutls/gnutls.h>
#include <melon/melon.h>
#include <gflags/gflags.h>

#include "init.hh"
#include "runtime/fiber.hh"

namespace mimosa
{
  static int gnutls_mutex_init(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    return ::melon_mutex_init(m, nullptr);
  }

  static int gnutls_mutex_lock(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    ::melon_mutex_lock(*m);
    return 0;
  }

  static int gnutls_mutex_unlock(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    ::melon_mutex_unlock(*m);
    return 0;
  }

  static int gnutls_mutex_deinit(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    ::melon_mutex_destroy(*m);
    return 0;
  }

  static void dummy(int)
  {
  }

  void init(int argc, char ** argv)
  {
    ::signal(SIGPIPE, dummy);

    if (::melon_init(0))
      throw int(0); // FIXME

    runtime::Fiber::start([&argv, &argc]() {
        google::ParseCommandLineFlags(&argc, &argv, true);
        ::gnutls_global_set_mutex(gnutls_mutex_init, gnutls_mutex_deinit,
                                  gnutls_mutex_lock, gnutls_mutex_unlock);
        ::gnutls_global_init();
      });
    ::melon_wait();
  }

  void wait()
  {
    ::melon_wait();
  }

  void deinit()
  {
    runtime::Fiber::start([]() {
        ::gnutls_global_deinit ();
        google::ShutDownCommandLineFlags();
      });
    ::melon_wait();
  }
}
