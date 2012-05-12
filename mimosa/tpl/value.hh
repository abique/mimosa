#ifndef MIMOSA_TPL_VALUE_HH
# define MIMOSA_TPL_VALUE_HH

# include "abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    template <typename T>
    class Value : public AbstractValue
    {
    public:
      MIMOSA_DEF_PTR(Value<T>);

      Value(const std::string &   name,
            const T &             value);

      Value(const T &             value);

      virtual const AbstractValue * lookup(const StringRef & var) const;
      virtual void write(stream::Stream::Ptr stream, Time timeout = 0) const;

      virtual Iterator::Ptr begin() const;
      virtual bool empty() const;

      T value_;
    };
  }
}

# include "value.hxx"

#endif /* !MIMOSA_TPL_VALUE_HH */
