#ifndef MIMOSA_URI_NORMALIZE_PATH_HH
# define MIMOSA_URI_NORMALIZE_PATH_HH

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

#endif /* !MIMOSA_URI_NORMALIZE_PATH_HH */
