#include "cache.hh"

namespace mimosa
{
  namespace tpl
  {
    Cache::Cache()
    {
      setEntryTimeout(runtime::minute * 15);
      setValueTimeout(runtime::hour);
      setCleanupPeriod(runtime::minute);
      startCleanupThread();
    }

    void
    Cache::cacheMiss(const std::string & key)
    {
      set(key, Template::parseFile(key));
    }
  }
}
