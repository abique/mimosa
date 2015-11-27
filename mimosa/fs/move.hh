#pragma once

#include <string>

namespace mimosa
{
  namespace fs
  {
    bool moveFile(const std::string &src,
                  const std::string &dst);
  }
}
