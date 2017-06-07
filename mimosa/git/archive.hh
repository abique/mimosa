#pragma once

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

