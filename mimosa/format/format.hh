#pragma once

# include <sstream>

namespace mimosa
{
  namespace format
  {
    inline
    std::ostream & format(std::ostream & os,
                          const char *   fmt);

    template <typename T, typename ... Args>
    inline
    std::ostream & format(std::ostream & os,
                          const char *   fmt,
                          const T &      value,
                          Args ...       args);

    template <typename ... Args>
    inline
    std::string str(const char *   fmt,
                    Args ...       args)
    {
      std::ostringstream os;
      format(os, fmt, args...);
      return os.str();
    }
  }
}

# include "format.hxx"

