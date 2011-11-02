#ifndef MIMOSA_LOG_ORIGIN_HH
# define MIMOSA_LOG_ORIGIN_HH

# include "../sync/spinlock.hh"
# include "level.hh"

namespace mimosa
{
  namespace log
  {
    struct Origin
    {
      Origin(const char * name, Level level);

      static Origin *       root_;
      static sync::SpinLock root_lock_;

      Origin * next_;
      Level    level_;
      char     name_[64];
    };
  }
}

#endif /* !MIMOSA_LOG_ORIGIN_HH */
