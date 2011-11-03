#ifndef MIMOSA_LOG_LOG_HH
# define MIMOSA_LOG_LOG_HH

# include "origin.hh"

namespace mimosa
{
  namespace log
  {
    void log(Level level, const Origin & origin, const std::string & msg);
  }
}

# define MIMOSA_LOG(Level, Origin, Fmt...)                              \
  do {                                                                  \
    if (Level >= Origin.level_ &&                                       \
        Level >= ::mimosa::log::current_level)                          \
      ::mimosa::log::log(Level, Origin, ::mimosa::format::str(Fmt));    \
  } while (0)

#endif /* !MIMOSA_LOG_LOG_HH */
