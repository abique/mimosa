#include "include.hh"

namespace mimosa
{
  namespace tpl
  {
    Include::Include(Template::ConstPtr  tpl,
                     const std::string & name)
      : AbstractValue(name),
        tpl_(tpl)
    {
    }

    const AbstractValue *
    Include::lookup(const string::StringRef & var) const
    {
      if (var == "." || var == name_)
        return this;
      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    void
    Include::write(stream::Stream::Ptr stream, runtime::Time timeout) const
    {
      tpl_->execute(stream, *this, timeout);
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
