#pragma once

#include "../expiring-cache.hh"
#include "template.hh"

namespace mimosa
{
  namespace tpl
  {
    class Cache : public mimosa::ExpiringCache<std::string, Template::Ptr>
    {
    public:
      Cache();
      virtual ~Cache() = default;

    protected:
      virtual void cacheMiss(const std::string & key);
    };
  }
}

extern template class mimosa::ExpiringCache<std::string, mimosa::tpl::Template::Ptr>;

