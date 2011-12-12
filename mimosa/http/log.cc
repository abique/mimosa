#include "log.hh"

namespace mimosa
{
  namespace http
  {
    log::Origin * http_log = new log::Origin("http", log::Warning);
  }
}
