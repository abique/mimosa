#include <cstdlib>

#include "env.hh"

namespace mimosa
{
  void setenv(const std::string &key, const std::string &value)
  {
#ifdef __unix__
    ::setenv(key.c_str(), value.c_str(), true);
#elif defined(__WIN32__)
    _putenv_s(key.c_str(), value.c_str());
#endif
  }
}
