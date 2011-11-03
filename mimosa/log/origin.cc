#include <cstring>

#include "origin.hh"

namespace mimosa
{
  namespace log
  {
    Origin::origins_type Origin::origins_;
    sync::Mutex       Origin::origins_lock_;

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
  }
}
