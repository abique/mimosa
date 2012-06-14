#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>

#include <cstring>
#include <cerrno>

#include "priviledge-drop.hh"
#include "options/options.hh"
#include "log/log.hh"

namespace mimosa
{
  static std::string * ROOT = nullptr;
  static std::string * UID  = nullptr;
  static std::string * GID  = nullptr;

  static const int __init = [] {
    uid_t uid = getuid();
    gid_t gid = getgid();

    // are we able to chroot and set{u,g}id()?
    if (uid > 0 && gid > 0)
      return 0;

    ROOT = options::addOption<std::string>("security", "chroot", "the path to chroot", "");
    UID  = options::addOption<std::string>("security", "uid", "the new uid", "");
    GID  = options::addOption<std::string>("security", "gid", "the new gid", "");
    return 0;
  }();

  static bool isNumber(const std::string & str)
  {
    for (auto & c : str)
      if (c < '0' || '9' < c)
        return false;
    return true;
  }

  void priviledgeDrop()
  {
    if (ROOT && !ROOT->empty()) {
      if (::chroot(ROOT->c_str()))
        log::error("chroot(%s) failed: %s", *ROOT, strerror(errno));
    }

    uid_t uid = getuid();
    gid_t gid = getgid();

    if (uid == 0 && UID && !UID->empty()) {
      if (!isNumber(*UID)) {
        struct passwd *p = getpwnam(UID->c_str());
        if (!p)
          log::error("getpwnam(%s) failed: %s", *UID, strerror(errno));
        else {
          uid = p->pw_uid;
          gid = p->pw_gid;
        }
      } else
        uid = atoi(UID->c_str());
    }

    if (uid == 0 && GID && !GID->empty()) {
      if (!isNumber(*GID)) {
        struct group *g = getgrnam(GID->c_str());
        if (!g)
          log::error("getgrnam(%s) failed: %s", *GID, strerror(errno));
        else
          gid = g->gr_gid;
      } else
        gid = atoi(GID->c_str());
    }

    if (setgid(gid))
      log::error("setgid(%d) failed: %s", gid, strerror(errno));
    if (setuid(uid))
      log::error("setuid(%d) failed: %s", uid, strerror(errno));
  }
}
