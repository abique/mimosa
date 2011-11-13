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
      virtual Value * lookup(const string::StringRef & var);
      virtual void write(stream::Stream::Ptr       stream);

      T       value_;
    };
  }
}

#endif /* !MIMOSA_TPL_VALUE_HH */
