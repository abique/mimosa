#include "cache.hh"
#include "../expiring-cache.hxx" // contains template definition

namespace mimosa
{
  namespace tpl
  {
    Cache::Cache()
    {
      setEntryTimeout(minute * 15);
      setValueTimeout(hour);
      setCleanupPeriod(minute);
      startCleanupThread();
    }

    void
    Cache::cacheMiss(const std::string & key)
    {
      set(key, Template::parseFile(key));
    }
  }
}

template class mimosa::ExpiringCache<std::string, mimosa::tpl::Template::Ptr>;
