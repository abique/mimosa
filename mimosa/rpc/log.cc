#include "log.hh"

namespace mimosa
{
  namespace rpc
  {
    log::Origin *rpc_log = new log::Origin("rpc", log::kInfo);
  }
}
