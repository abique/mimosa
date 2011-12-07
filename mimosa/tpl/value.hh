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

      Value(const T &             value,
            const std::string &   name = "",
            const AbstractValue * parent = nullptr);

      virtual const AbstractValue * lookup(const string::StringRef & var) const;
      virtual void write(stream::Stream::Ptr stream, runtime::Time timeout = 0) const;

      virtual Iterator::Ptr begin() const;
      virtual bool empty() const;

      T value_;
    };
  }
}

# include "value.hxx"

#endif /* !MIMOSA_TPL_VALUE_HH */
