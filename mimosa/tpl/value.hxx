#include "../format/format-stream.hh"
#include "value.hh"

namespace mimosa
{
  namespace tpl
  {
    template <typename T>
    Value::Value(const T & value, AbstractValue * parent)
      : AbstractValue(parent),
        value_(value)
    {
    }

    template <typename T>
    const AbstractValue *
    Value::lookup(const string::StringRef & var) const
    {
      if (var == ".")
        return this;
      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    template <typename T>
    void
    Value::write(stream::Stream::Ptr stream) const
    {
      format::format(stream, "%v", value_);
    }

    template <typename T>
    Iterator::Ptr
    Value::begin() const
    {
      return new SingleValueIterator(this);
    }

    template <typename T>
    bool
    Value::empty() const
    {
      return false;
    }
  }
}
