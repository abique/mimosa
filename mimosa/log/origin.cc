#include <cstring>

#include "origin.hh"

namespace mimosa
{
  namespace log
  {
    Origin * Origin::root_ = nullptr;

    Origin::Origin(const char * name, Level level)
    {
      /* setup the linked list */
      {
        sync::SpinLock::Locker locker(root_lock_);
        next_ = root_;
        root_ = this;
      }

      /* copy the name */
      ::strncpy(name_, name, sizeof (name_));
      name_[sizeof (name_) - 1] = '\0';

      /* copy the default log level */
      level_ = level;
    }
  }
}
