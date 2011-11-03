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
      Origin(const char * name, Level level);
      ~Origin();

      typedef container::IntrusiveDlistHook<Origin *>        root_hook_type;

      root_hook_type root_hook_;
      Level          level_;
      char           name_[64];

      typedef container::IntrusiveDlist<Origin,
                                        Origin *,
                                        &Origin::root_hook_> root_type;

      static root_type   root_;
      static sync::Mutex root_lock_;
    };
  }
}

#endif /* !MIMOSA_LOG_ORIGIN_HH */
