#ifndef MIMOSA_LOG_LOG_HH
# define MIMOSA_LOG_LOG_HH

# include "origin.hh"
# include "../format/format.hh"

namespace mimosa
{
  namespace log
  {
    void log(Level level, const Origin & origin, const std::string & msg);
  }
}

# define MIMOSA_LOG(Level, Origin, Fmt...)                              \
  do {                                                                  \
    if (::mimosa::log::Level >= Origin.level_ &&                        \
        ::mimosa::log::Level >= ::mimosa::log::current_level)           \
      ::mimosa::log::log(::mimosa::log::Level, Origin,                  \
                         ::mimosa::format::str(Fmt));                   \
  } while (0)

#endif /* !MIMOSA_LOG_LOG_HH */
