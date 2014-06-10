#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>

#include <cstring>
#include <cerrno>
#include <string>

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

  bool priviledgeDrop(const std::string & chroot,
                      const std::string & user,
                      const std::string & group)
  {
    if (!chroot.empty()) {
      if (::chroot(chroot.c_str())) {
        log::error("chroot(%s) failed: %s", chroot, strerror(errno));
        return false;
      }
    }

    uid_t uid = getuid();
    gid_t gid = getgid();

    if (uid == 0 && !user.empty()) {
      if (!isNumber(user)) {
        struct passwd *p = getpwnam(user.c_str());
        if (!p) {
          log::error("getpwnam(%s) failed: %s", user, strerror(errno));
          return false;
        } else {
          uid = p->pw_uid;
          gid = p->pw_gid;
        }
      } else
        uid = atoi(user.c_str());
    }

    if (uid == 0 && !group.empty()) {
      if (!isNumber(group)) {
        struct group *g = getgrnam(group.c_str());
        if (!g) {
          log::error("getgrnam(%s) failed: %s", group, strerror(errno));
          return false;
        } else
          gid = g->gr_gid;
      } else
        gid = atoi(group.c_str());
    }

    if (setgid(gid)) {
      log::error("setgid(%d) failed: %s", gid, strerror(errno));
      return false;
    }

    if (setuid(uid)) {
      log::error("setuid(%d) failed: %s", uid, strerror(errno));
      return false;
    }
    return true;
  }

  bool priviledgeDrop()
  {
    if (!ROOT || !UID || !GID)
      return false;
    return priviledgeDrop(*ROOT, *UID, *GID);
  }
}
