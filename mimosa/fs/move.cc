#include <cstdio>

#include "move.hh"
#include "copy.hh"
#include "rm.hh"

namespace mimosa
{
  namespace fs
  {
    bool moveFile(const std::string &src,
                  const std::string &dst)
    {
      if (!::rename(src.c_str(), dst.c_str()))
        return true;

      if (errno == EXDEV)
      {
        if (!copyFile(src, dst))
          return false;
        if (!rm(src, false, true)) {
            rm(dst, false, true);
            return false;
        }
        return true;
      }
      return false;
    }
  }
}
