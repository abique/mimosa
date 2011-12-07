#include "list.hh"

namespace mimosa
{
  namespace tpl
  {
    List::List(const std::string &   name,
               const AbstractValue * parent)
      : AbstractValue(name, parent)
    {
    }

    const AbstractValue *
    List::lookup(const string::StringRef & var) const
    {
      if (var == "." || var == name_)
        return this;
      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    void
    List::write(stream::Stream::Ptr /*stream*/,
                runtime::Time       /*timeout*/) const
    {
    }

    AbstractValue::Iterator::Ptr
    List::begin() const
    {
      return new ListIterator(this, values_.begin());
    }

    bool
    List::empty() const
    {
      return values_.empty();
    }

    List::ListIterator::ListIterator(const List *                        value,
                                     const values_type::const_iterator & it)
      : value_(value),
        it_(it)
    {
    }

    void
    List::ListIterator::next()
    {
      ++it_;
    }

    bool
    List::ListIterator::end() const
    {
      return it_ == value_->values_.end();
    }
  }
}
