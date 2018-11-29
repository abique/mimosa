#include <utility>

#include "include.hh"

namespace mimosa
{
  namespace tpl
  {
    Include::Include(const std::string & name,
                     Template::ConstPtr  tpl)
      : AbstractValue(name),
        tpl_(std::move(tpl))
    {
    }

    const AbstractValue *
    Include::lookup(const StringRef & var) const
    {
      if (var == "." || var == name_)
        return this;
      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    void
    Include::write(stream::Stream::Ptr stream) const
    {
      tpl_->execute(stream, *this);
    }

    AbstractValue::Iterator::Ptr
    Include::begin() const
    {
      return new AbstractValue::SingleValueIterator(this);
    }

    bool
    Include::empty() const
    {
      return false;
    }
  }
}
