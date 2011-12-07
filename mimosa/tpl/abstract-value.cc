#include "abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    AbstractValue::AbstractValue(const std::string &   name)
      : parent_(nullptr),
        name_(name)
    {
    }

    AbstractValue::SingleValueIterator::SingleValueIterator(const AbstractValue * value)
      : value_(value)
    {
    }

    const AbstractValue *
    AbstractValue::SingleValueIterator::value() const
    {
      return value_;
    }

    void
    AbstractValue::SingleValueIterator::next()
    {
      value_ = nullptr;
    }

    bool
    AbstractValue::SingleValueIterator::end() const
    {
      return !value_;
    }
  }
}
