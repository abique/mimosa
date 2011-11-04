#ifndef MIMOSA_LOG_LEVEL_HH
# define MIMOSA_LOG_LEVEL_HH

# include <string>

namespace mimosa
{
  namespace log
  {
    enum Level
    {
      Debug    = 0, ///< here goes debug information
      Info     = 1, ///< here goes information messages
      Warning  = 2, ///< here goes warning
      Error    = 3, ///< here goes unexpected errors
      Critical = 4, ///< here goes events which should never happend (like an assert)
                    ///  but are recoverable
      Fatal    = 5, ///< here goes fatal errors, after that the program should exit
    };

    extern Level current_level;

    /// converts a level into a string
    const char * levelName(Level level);

    /// converts a string into a level, fallback to Info when parse failed
    Level parseLevel(const char * str);
  }
}

#endif /* !MIMOSA_LOG_LEVEL_HH */
