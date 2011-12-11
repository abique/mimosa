#ifndef MIMOSA_TPL_FILTER_FACTORY_HH
# define MIMOSA_TPL_FILTER_FACTORY_HH

# include <unordered_map>
# include <string>

# include "../sync/rwlock.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace tpl
  {
    class FilterFactory
    {
    public:
      typedef stream::Stream::Ptr (*creator_type)(stream::Stream::Ptr);

      FilterFactory();
      ~FilterFactory();

      static FilterFactory & instance();

      stream::Stream::Ptr create(const std::string & filter,
                                 stream::Stream::Ptr stream) const;

      bool registerFilter(const std::string & filter,
                          creator_type        creator);

    private:
      mutable sync::RWLock                          rwlock_;
      std::unordered_map<std::string, creator_type> creators_;
    };
  }
}

#endif /* !MIMOSA_TPL_FILTER_FACTORY_HH */
