#ifndef MIMOSA_TPL_CACHE_HH
# define MIMOSA_TPL_CACHE_HH

# include "../cache.hh"
# include "template.hh"

namespace mimosa
{
  namespace tpl
  {
    class Cache : public mimosa::Cache<std::string, Template::Ptr>
    {
    public:
      Cache();

    protected:
      virtual void cacheMiss(const std::string & key);
    };
  }
}

extern template class mimosa::Cache<std::string, mimosa::tpl::Template::Ptr>;

#endif /* !MIMOSA_TPL_CACHE_HH */
