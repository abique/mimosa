#pragma once

# include <unordered_map>
# include <string>

# include "../shared-mutex.hh"
# include "../stream/filter.hh"

namespace mimosa
{
  namespace tpl
  {
    class FilterFactory
    {
    public:
      typedef stream::Filter::Ptr (*creator_type)(stream::Stream::Ptr);

      FilterFactory();
      ~FilterFactory();

      static FilterFactory & instance();

      stream::Stream::Ptr create(const std::string & filter,
                                 stream::Stream::Ptr stream) const;

      bool registerFilter(const std::string & filter,
                          creator_type        creator);

    private:
      mutable SharedMutex                           rwlock_;
      std::unordered_map<std::string, creator_type> creators_;
    };
  }
}

