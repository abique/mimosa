#include <cstring>
#include <iostream>

#include "../options/options.hh"
#include "../container/singleton.hh"
#include "origin.hh"
#include "log.hh"

namespace mimosa
{
  namespace log
  {
    struct Origins : public container::Singleton<Origins>,
                     public NonCopyable
    {
      Origin::origins_type list_;
      sync::Mutex          lock_;
    };

    Origin::Origin(const char * name, Level level)
    {
      /* setup the linked list */
      {
        sync::Mutex::Locker locker(Origins::instance().lock_);
        Origins::instance().list_.pushBack(this);
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
      sync::Mutex::Locker locker(Origins::instance().lock_);
      Origins::instance().list_.erase(this);
    }

    void
    Origin::setLevel(const std::string & origin, Level level)
    {
      sync::Mutex::Locker locker(Origins::instance().lock_);

      for (auto it = Origins::instance().list_.begin(); it != Origins::instance().list_.end(); ++it)
        if (!::strcasecmp(origin.c_str(), it->name_))
        {
          it->level_ = level;
          return;
        }

      log::warning("origin not found: `%s'", origin);
    }

    void
    Origin::filter(const std::string & filter)
    {
      const char * str = filter.c_str();

      while (*str)
      {
        Level level = kInfo;
        std::string origin_name;

        // checking for `+' or `-'
        if (*str == '+')
        {
          level = kDebug;
          ++str;
        }
        else if (*str == '-')
        {
          level = kCritical;
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
          str = end;
        }

        if (*str == ',')
          ++str;

        setLevel(origin_name, level);
      }
    }

    namespace
    {
      class OptionFilter : public options::BasicOption
      {
      public:
        OptionFilter()
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

      bool dummy = options::addBasicOption(new OptionFilter);

      class OptionOrigins : public options::BasicOption
      {
      public:
        OptionOrigins()
          : options::BasicOption("log", "log-origins", "lists available origins")
        {
        }

        virtual bool parse(int & argc, char **& argv)
        {
          auto & list = Origins::instance().list_;
          for (auto it = list.begin(); it != list.end(); ++it)
            std::cout << " - " << it->name_ << " (" << levelName(it->level_)
                      << ")" << std::endl;
          ::exit(0);
          return true;
        }
      };

      bool dummy2 = options::addBasicOption(new OptionOrigins);
    }
  }
}
