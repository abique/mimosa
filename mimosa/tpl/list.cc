#include "list.hh"

namespace mimosa
{
  namespace tpl
  {
    List::List(const std::string & name)
      : AbstractValue(name)
    {
    }

    const AbstractValue *
    List::lookup(const StringRef & var) const
    {
      if (var == "." || var == name_)
        return this;
      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    void
    List::write(stream::Stream::Ptr /*stream*/,
                Time       /*timeout*/) const
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

    const AbstractValue *
    List::ListIterator::value() const
    {
      return it_->get();
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
