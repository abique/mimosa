#include <cstdlib>

#include "env.hh"

namespace mimosa
{
  void setenv(const std::string &key, const std::string &value)
  {
#if defined(__unix__) || defined(__APPLE__)
    ::setenv(key.c_str(), value.c_str(), true);
#elif defined(__WIN32__)
    _putenv_s(key.c_str(), value.c_str());
#endif
  }
}
