#include <utility>

#include "log.hh"
#include "filter-factory.hh"
#include "../stream/html-escape.hh"
#include "../stream/percent-encoder.hh"
#include "../stream/base16-encoder.hh"

namespace mimosa
{
  namespace tpl
  {
    static stream::Filter::Ptr createHtmlFilter(stream::Stream::Ptr stream)
    {
      return new stream::HtmlEscape(std::move(stream));
    }

    static stream::Filter::Ptr createPercentEncoder(stream::Stream::Ptr stream)
    {
      return new stream::PercentEncoder(std::move(stream), uri::kRfc3986);
    }

    static stream::Filter::Ptr createBase16Encoder(stream::Stream::Ptr stream)
    {
      return new stream::Base16Encoder(std::move(stream));
    }

    FilterFactory::FilterFactory()
    {
      // register default filters
      registerFilter("h", createHtmlFilter);
      registerFilter("html", createHtmlFilter);
      registerFilter("u", createPercentEncoder);
      registerFilter("url", createPercentEncoder);
      registerFilter("b16", createBase16Encoder);
    }

    FilterFactory::~FilterFactory()
    {
      // wait for other lock
      SharedMutex::Locker locker(rwlock_);
    }

    stream::Stream::Ptr
    FilterFactory::create(const std::string & filter,
                          stream::Stream::Ptr stream) const
    {
      SharedMutex::ReadLocker locker(rwlock_);

      auto it = creators_.find(filter);
      if (it != creators_.end())
        return it->second(stream);

      tpl_log->warning("%v: filter not found", filter);
      return stream;
    }

    bool
    FilterFactory::registerFilter(const std::string & filter,
                                  creator_type        creator)
    {
      SharedMutex::Locker locker(rwlock_);

      auto it = creators_.find(filter);
      if (it != creators_.end())
      {
        tpl_log->warning("%v: duplicate filter definition", filter);
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
