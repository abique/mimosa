#include "../format/format-stream.hh"
#include "value.hh"

namespace mimosa
{
  namespace tpl
  {
    template <typename T>
    Value<T>::Value(const T &             value)
      : AbstractValue(""),
        value_(value)
    {
    }

    template <typename T>
    Value<T>::Value(const std::string &   name,
                    const T &             value)
      : AbstractValue(name),
        value_(value)
    {
    }

    template <typename T>
    const AbstractValue *
    Value<T>::lookup(const StringRef & var) const
    {
      if (var == "." || var == name_)
        return this;
      if (parent_)
        return parent_->lookup(var);
      return nullptr;
    }

    template <typename T>
    void
    Value<T>::write(stream::Stream::Ptr stream, Time timeout) const
    {
      format::format(stream, timeout, "%v", value_);
    }

    template <typename T>
    AbstractValue::Iterator::Ptr
    Value<T>::begin() const
    {
      return new AbstractValue::SingleValueIterator(this);
    }

    template <typename T>
    bool
    Value<T>::empty() const
    {
      return false;
    }
  }
}
