#include "dict.hh"

namespace mimosa
{
  namespace tpl
  {
    Dict::Dict(const std::string & name)
      : AbstractValue(name)
    {
    }

    const AbstractValue *
    Dict::lookup(const StringRef & var) const
    {
      if (var == ".")
        return this;

      auto it = values_.find((std::string)var);
      if (it != values_.end())
        return it->second.get();

      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    void
    Dict::write(stream::Stream::Ptr /*stream*/) const
    {
    }

    AbstractValue::Iterator::Ptr
    Dict::begin() const
    {
      return new DictIterator(this, values_.begin());
    }

    bool
    Dict::empty() const
    {
      return values_.empty();
    }

    Dict::DictIterator::DictIterator(const Dict *                        value,
                                     const values_type::const_iterator & it)
      : value_(value),
        it_(it)
    {
    }

    const AbstractValue *
    Dict::DictIterator::value() const
    {
      return it_->second.get();
    }

    void
    Dict::DictIterator::next()
    {
      ++it_;
    }

    bool
    Dict::DictIterator::end() const
    {
      return it_ == value_->values_.end();
    }
  }
}
