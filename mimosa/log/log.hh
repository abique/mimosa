#ifndef MIMOSA_LOG_LOG_HH
# define MIMOSA_LOG_LOG_HH

# include <string>

# include "../format/format.hh"
# include "level.hh"

namespace mimosa
{
  namespace log
  {
    class Origin;

    void log(Level level, const Origin * origin, const std::string & msg);

    /**
     * Closes the log file descriptor if the option -log-file is not empty.
     * The next call to log() will re-open it.
     */
    void release();

# define IMPL_LOG(Level, Name)                          \
    template <typename ... Args>                        \
    inline void                                         \
    Name(const char * fmt, Args ... args)               \
    {                                                   \
      if (Level >= global_level)                        \
        log(Level, nullptr, format::str(fmt, args...)); \
    }

    IMPL_LOG(kDebug, debug);
    IMPL_LOG(kInfo, info);
    IMPL_LOG(kWarning, warning);
    IMPL_LOG(kError, error);
    IMPL_LOG(kCritical, critical);
    IMPL_LOG(kFatal, fatal);

# undef IMPL_LOG

  }
}

# include "origin.hh"

#endif /* !MIMOSA_LOG_LOG_HH */
