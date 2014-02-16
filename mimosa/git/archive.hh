#ifndef MIMOSA_GIT_ARCHIVE_HH
# define MIMOSA_GIT_ARCHIVE_HH

# include "../archive/writer.hh"
# include "tree.hh"

namespace mimosa
{
  namespace git
  {
    bool gitArchive(Tree &                    tree,
                    mimosa::archive::Writer & archive,
                    const std::string &       prefix = "");
  }
}

#endif /* !MIMOSA_GIT_ARCHIVE_HH */
