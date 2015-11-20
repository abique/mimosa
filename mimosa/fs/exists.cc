#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "exists.hh"

namespace mimosa
{
  namespace fs
  {
    bool exists(const std::string &path)
    {
      struct ::stat st;
      return ::stat(path.c_str(), &st) == 0;
    }
  }
}
