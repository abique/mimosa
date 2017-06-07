#pragma once

# include <string>

namespace mimosa
{
  namespace log
  {
    enum Level
    {
      kDebug    = 0, ///< here goes debug information
      kInfo     = 1, ///< here goes information messages
      kWarning  = 2, ///< here goes warning
      kError    = 3, ///< here goes unexpected errors
      kCritical = 4, ///< here goes events which should never happend (like an assert)
                    ///  but are recoverable
      kFatal    = 5, ///< here goes fatal errors, after that the program should exit
    };

    extern Level global_level;

    /// converts a level into a string
    const char * levelName(Level level);

    /// converts a string into a level, fallback to Info when parse failed
    Level parseLevel(const char * str);
  }
}

