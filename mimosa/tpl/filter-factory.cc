#include "log.hh"
#include "filter-factory.hh"
#include "../stream/html-escape.hh"

namespace mimosa
{
  namespace tpl
  {
    static stream::Filter::Ptr createHtmlFilter(stream::Stream::Ptr stream)
    {
      return new stream::HtmlEscape(stream);
    }

    FilterFactory::FilterFactory()
    {
      // register default filters
      registerFilter("h", createHtmlFilter);
      registerFilter("html", createHtmlFilter);
    }

    FilterFactory::~FilterFactory()
    {
      // wait for other lock
      sync::RWLock::Locker locker(rwlock_);
    }

    stream::Stream::Ptr
    FilterFactory::create(const std::string & filter,
                          stream::Stream::Ptr stream) const
    {
      sync::RWLock::ReadLocker locker(rwlock_);

      auto it = creators_.find(filter);
      if (it != creators_.end())
        return it->second(stream);

      MIMOSA_LOG(Warning, tpl_log, "%v: filter not found", filter);
      return stream;
    }

    bool
    FilterFactory::registerFilter(const std::string & filter,
                                  creator_type        creator)
    {
      sync::RWLock::Locker locker(rwlock_);

      auto it = creators_.find(filter);
      if (it != creators_.end())
      {
        MIMOSA_LOG(Warning, tpl_log, "%v: duplicate filter definition", filter);
        return false;
      }

      creators_[filter] = creator;
      return true;
    }

    FilterFactory &
    FilterFactory::instance()
    {
      static FilterFactory factory;
      return factory;
    }
  }
}
