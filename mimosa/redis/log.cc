#include "log.hh"

namespace mimosa
{
  namespace redis
  {
    log::Origin * redis_log = new log::Origin("redis", log::kWarning);
  }
}
