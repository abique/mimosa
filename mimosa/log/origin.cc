#include <cstring>

#include "origin.hh"

namespace mimosa
{
  namespace log
  {
    Origin::root_type   Origin::root_;
    sync::Mutex         Origin::root_lock_;

    Origin::Origin(const char * name, Level level)
    {
      /* setup the linked list */
      {
        sync::Mutex::Locker locker(root_lock_);
        root_.pushBack(this);
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
      sync::Mutex::Locker locker(root_lock_);
      root_.erase(this);
    }
  }
}
