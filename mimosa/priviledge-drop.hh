#ifndef MIMOSA_PRIVILEDGE_DROP_HH
# define MIMOSA_PRIVILEDGE_DROP_HH

namespace mimosa
{
  bool priviledgeDrop(const std::string & chroot,
                      const std::string & user,
                      const std::string & group);

  /**
   * This function registers options for chroot and userid/groupid.
   *
   * When you call it, it will check if we are root, and then chroot
   * and change user/group.
   */
  bool priviledgeDrop();
}

#endif /* !MIMOSA_PRIVILEDGE_DROP_HH */
