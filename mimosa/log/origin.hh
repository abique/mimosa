#ifndef MIMOSA_LOG_ORIGIN_HH
# define MIMOSA_LOG_ORIGIN_HH

# include "../sync/mutex.hh"
# include "../container/intrusive-dlist.hh"
# include "level.hh"

namespace mimosa
{
  namespace log
  {
    struct Origin
    {
    public:
      Origin(const char * name, Level level);
      ~Origin();

      Level level_;
      char  name_[64];

      static void filter(const std::string & filter);
      static void setLevel(const std::string & origin, Level level);

    private:

      typedef container::IntrusiveDlistHook<Origin *> origins_hook_type;
      origins_hook_type origins_hook_;

      typedef container::IntrusiveDlist<Origin,
                                        Origin *,
                                        &Origin::origins_hook_> origins_type;
      static origins_type origins_;
      static sync::Mutex  origins_lock_;
    };
  }
}

#endif /* !MIMOSA_LOG_ORIGIN_HH */
