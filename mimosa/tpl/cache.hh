#ifndef MIMOSA_TPL_CACHE_HH
# define MIMOSA_TPL_CACHE_HH

# include "../container/cache.hh"
# include "template.hh"

namespace mimosa
{
  namespace tpl
  {
    class Cache : public container::Cache<std::string, Template::Ptr>
    {
    public:
      Cache();

    protected:
      virtual void cacheMiss(const std::string & key);
    };
  }
}

#endif /* !MIMOSA_TPL_CACHE_HH */
