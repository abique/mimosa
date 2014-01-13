#ifndef MIMOSA_FS_RM_HH
# define MIMOSA_FS_RM_HH

# include <string>

namespace mimosa
{
  namespace fs
  {
    bool rm(const std::string & path,
            bool                recursive = false,
            bool                force     = false);
  }
}

#endif /* !MIMOSA_FS_RM_HH */
