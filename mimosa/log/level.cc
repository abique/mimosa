#include <cstring>

#include "../options/options.hh"
#include "level.hh"

namespace mimosa
{
  namespace log
  {
    namespace
    {
      class Option : public options::BasicOption
      {
      public:
        Option()
          : options::BasicOption("log", "log-level", "set the minimum log level,"
                                 " possible values are debug, info, warning,"
                                 " error, critical and fatal")
        {
        }

        virtual bool parse(int & argc, char **& argv)
        {
          if (argc < 1)
            return false;

          global_level = parseLevel(*argv);
          --argc;
          ++argv;
          return true;
        }

        virtual void showDesc(std::ostream & os)
        {
          os << "  -" << name_ << " [=" << levelName(global_level) << "] (" << desc_ << ")" << std::endl;
        }
      };

      bool dummy = options::addBasicOption(new Option);
    }

    Level global_level = kWarning;

    const char * const level_name[] = {
      "debug",
      "info",
      "warning",
      "error",
      "critical",
      "fatal"
    };

    const char * levelName(Level level)
    {
      return level_name[level];
    }

    Level parseLevel(const char * str)
    {
      for (int i = 0; i <= 5; ++i)
        if (!::strcasecmp(level_name[i], str))
          return static_cast<Level> (i);
      return kInfo;
    }
  }
}
