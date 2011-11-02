#ifndef MIMOSA_LOG_LEVEL_HH
# define MIMOSA_LOG_LEVEL_HH

# include <string>

namespace mimosa
{
  namespace log
  {
    enum Level
    {
      Debug,    ///< here goes debug information
      Info,     ///< here goes information messages
      Warning,  ///< here goes warning
      Error,    ///< here goes unexpected errors
      Critical, ///< here goes events which should never happend (like an assert)
                ///  but are recoverable
      Fatal,    ///< here goes fatal errors, after that the program should exit
    };

    extern Level current_level;

    /// converts a level into a string
    const char * levelName(Level level);

    /// converts a string into a level, fallback to Info when parse failed
    Level parseLevel(const std::string & level);
  }
}

#endif /* !MIMOSA_LOG_LEVEL_HH */
