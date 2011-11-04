#include <cstring>

#include "../options/options.hh"
#include "origin.hh"
#include "log.hh"

namespace mimosa
{
  namespace log
  {
    Origin::origins_type Origin::origins_;
    sync::Mutex          Origin::origins_lock_;

    Origin::Origin(const char * name, Level level)
    {
      /* setup the linked list */
      {
        sync::Mutex::Locker locker(origins_lock_);
        origins_.pushBack(this);
      }

      /* copy the name */
      ::strncpy(name_, name, sizeof (name_));
      name_[sizeof (name_) - 1] = '\0';

      /* copy the default log level */
      level_ = level;
    }

    Origin::~Origin()
    {
      /* setup the linked list */
      sync::Mutex::Locker locker(origins_lock_);
      origins_.erase(this);
    }

    void
    Origin::setLevel(const std::string & origin, Level level)
    {
      sync::Mutex::Locker locker(origins_lock_);

      for (auto it = origins_.begin(); it != origins_.end(); ++it)
        if (!::strcasecmp(origin.c_str(), it->name_))
        {
          it->level_ = level;
          return;
        }

      MIMOSA_LOG(Warning, NULL, "origin not found: `%s'", origin);
    }

    void
    Origin::filter(const std::string & filter)
    {
      const char * str = filter.c_str();

      while (*str)
      {
        Level level = Info;
        std::string origin_name;

        // checking for `+' or `-'
        if (*str == '+')
        {
          level = Debug;
          ++str;
        }
        else if (*str == '-')
        {
          level = Critical;
          ++str;
        }

        // getting origin's name
        const char * end = str + ::strcspn(str, ":,");
        origin_name.assign(str, end - str);
        str = end;

        // getting level
        if (*str == ':')
        {
          ++str;
          const char * end = str + ::strcspn(str, ",");
          std::string level_name(str, end - str);
          level = parseLevel(level_name.c_str());
        }

        if (*str == ',')
          ++str;

        setLevel(origin_name, level);
      }
    }

    namespace
    {
      class Option : public options::BasicOption
      {
      public:
        Option()
          : options::BasicOption("log", "log-filter", "set the log filtering string: "
                                 "origin:level,... +origin is equivalent to origin:debug,"
                                 " and -origin is equivalent to origin:critical.")
        {
        }

        virtual bool parse(int & argc, char **& argv)
        {
          if (argc < 1)
            return false;

          Origin::filter(*argv);

          --argc;
          ++argv;
          return true;
        }
      };

      bool dummy = options::addBasicOption(new Option);
    }
  }
}
