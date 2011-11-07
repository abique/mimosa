#include "log.hh"

namespace mimosa
{
  namespace http
  {
    log::Origin * http = new log::Origin("http", log::Warning);
  }
}
