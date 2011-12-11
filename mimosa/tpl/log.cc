#include "log.hh"

namespace mimosa
{
  namespace tpl
  {
    mimosa::log::Origin * tpl_log = new mimosa::log::Origin("template", mimosa::log::Warning);
  }
}
