#pragma once

# include <cstdint>
# include <string>

namespace mimosa
{
  namespace uri
  {
    void normalizePath(const char *  input,
                       uint32_t      len,
                       std::string * output);
  }
}

