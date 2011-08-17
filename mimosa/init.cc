#include <cassert>
#include <cerrno>
#include <gnutls/gnutls.h>
#include <melon/melon.h>
#include <gflags/gflags.h>

#include "init.hh"

namespace mimosa
{
#if 0
  static int gnutls_mutex_init(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    assert(m);
    *m = ::melon_mutex_new(0);
    return !*m;
  }

  static int gnutls_mutex_lock(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    assert(*m);
    ::melon_mutex_lock(*m);
    return 0;
  }

  static int gnutls_mutex_unlock(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    assert(*m);
    ::melon_mutex_unlock(*m);
    return 0;
  }

  static int gnutls_mutex_deinit(void ** mutex)
  {
    ::melon_mutex ** m = reinterpret_cast< ::melon_mutex **>(mutex);
    assert(*m);
    ::melon_mutex_destroy(*m);
    return 0;
  }
#endif

  void init(int argc, char ** argv)
  {
    if (::melon_init(0))
      throw int(0); // FIXME

    google::ParseCommandLineFlags(&argc, &argv, true);
    // ::gnutls_global_set_mutex(gnutls_mutex_init, gnutls_mutex_lock,
    //                           gnutls_mutex_unlock, gnutls_mutex_deinit);
    // ::gnutls_global_init();
  }

  void wait()
  {
    ::melon_wait();
  }

  void deinit()
  {
    //::gnutls_global_deinit ();
    google::ShutDownCommandLineFlags();
    ::melon_deinit();
  }
}
