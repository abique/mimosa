#pragma once

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

