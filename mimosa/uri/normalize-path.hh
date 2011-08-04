#ifndef MIMOSA_URI_NORMALIZE_PATH_HH
# define MIMOSA_URI_NORMALIZE_PATH_HH

# include <string>

# include "../string/string-ref.hh"

namespace mimosa
{
  namespace uri
  {
    void normalizePath(const string::StringRef & path,
                       std::string *             output);
  }
}
#endif /* !MIMOSA_URI_NORMALIZE_PATH_HH */
