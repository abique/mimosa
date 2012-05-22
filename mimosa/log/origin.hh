#ifndef MIMOSA_LOG_ORIGIN_HH
# define MIMOSA_LOG_ORIGIN_HH

# include "../format/format.hh"
# include "../mutex.hh"
# include "../intrusive-dlist.hh"
# include "level.hh"
# include "log.hh"

namespace mimosa
{
  namespace log
  {
    class Origin : private NonCopyable
    {
    public:
      Origin(const char * name, Level level);
      ~Origin();

      Level level_;
      char  name_[64];

      static void filter(const std::string & filter);
      static void setLevel(const std::string & origin, Level level);

      typedef IntrusiveDListHook<Origin *> origins_hook_type;

# define IMPL_LOG(Level, Name)                          \
      template <typename ... Args>                      \
      inline void                                       \
      Name(const char * fmt, Args ... args)             \
      {                                                 \
        if (Level >= level_ && Level >= global_level)   \
          log(Level, this, format::str(fmt, args...));  \
      }

      IMPL_LOG(kDebug, debug);
      IMPL_LOG(kInfo, info);
      IMPL_LOG(kWarning, warning);
      IMPL_LOG(kError, error);
      IMPL_LOG(kCritical, critical);
      IMPL_LOG(kFatal, fatal);

# undef IMPL_LOG

    private:
      origins_hook_type origins_hook_;

    public:
      typedef IntrusiveDList<Origin,
                                        Origin *,
                                        &Origin::origins_hook_> origins_type;
    };
  }
}

#endif /* !MIMOSA_LOG_ORIGIN_HH */
