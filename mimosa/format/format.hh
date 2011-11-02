#ifndef MIMOSA_FORMAT_FORMAT_HH
# define MIMOSA_FORMAT_FORMAT_HH

# include <ostream>

namespace mimosa
{
  namespace format
  {
    std::ostream & format(std::ostream & os,
                          const char *   fmt);

    template <typename ... Args>
    std::ostream & format(std::ostream & os,
                          const char *   fmt,
                          T &            value,
                          Args ...       args);
  }
}

# include "format.hxx"

#endif /* !MIMOSA_FORMAT_FORMAT_HH */
