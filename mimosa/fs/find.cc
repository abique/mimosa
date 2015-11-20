#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "find.hh"
#include "dir-iterator.hh"

namespace mimosa
{
  namespace fs
  {
    void
    find(const std::string &                                    root,
         uint32_t                                               max_depth,
         const std::function<void (const std::string & path)> & cb)
    {
      DirIterator it(root);

      if (!it.ok())
        return;

      for (; !it.end(); ++it)
      {
        std::string path = it.entryPath();
        cb(path);

        if (max_depth == 0)
          continue;

        struct stat st;
        if (::stat(path.c_str(), &st))
          continue;

        if (S_ISDIR(st.st_mode))
          find(path, max_depth - 1, cb);
      }
    }
  }
}
