#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>

#include "dir-iterator.hh"
#include "rm.hh"

namespace mimosa
{
  namespace fs
  {
    bool
    rm(const std::string & path, bool recursive, bool force)
    {
      struct ::stat st;

      if (::stat(path.c_str(), &st))
        return false || force;

      if (S_ISREG(st.st_mode) ||
          S_ISFIFO(st.st_mode) ||
#ifndef __WIN32__
          S_ISSOCK(st.st_mode) ||
          S_ISLNK(st.st_mode) ||
#endif
          S_ISBLK(st.st_mode) ||
          S_ISCHR(st.st_mode)) {
        if (!::unlink(path.c_str()))
          return true;
        if (!force || errno != EPERM)
          return false;
        if (::chmod(path.c_str(), 0700))
          return false;
        return !::unlink(path.c_str());
      }

      if (S_ISDIR(st.st_mode)) {
        if (recursive)
          for (DirIterator it(path); !it.end(); ++it)
            rm(it.entryPath(), true, force);
        if (!::rmdir(path.c_str()))
          return true;
        if (!force || errno != EPERM)
          return false;
        if (::chmod(path.c_str(), 0700))
          return false;
        return !::rmdir(path.c_str());
      }

      return false;
    }
  }
}
