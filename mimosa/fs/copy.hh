#pragma once

#include <string>

namespace mimosa
{
  namespace fs
  {
    bool copyFile(const std::string &src,
                  const std::string &dst);
  }
}
