#include <utility>

#include "abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    AbstractValue::AbstractValue(std::string    name)
      : parent_(nullptr),
        name_(std::move(name))
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
