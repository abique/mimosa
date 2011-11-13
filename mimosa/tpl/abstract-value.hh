#ifndef MIMOSA_TPL_ABSTRACT_VALUE_HH
# define MIMOSA_TPL_ABSTRACT_VALUE_HH

# include "../../ref-countable.hh"
# include "../../stream/stream.hh"

namespace mimosa
{
  namespace tpl
  {
    class AbstractValue
    {
    public:
      virtual const Value * lookup(const string::StringRef & var) = 0;
      virtual void write(stream::Stream::Ptr       stream) = 0;

      Value * parent_;
    };
  }
}

#endif /* !MIMOSA_TPL_ABSTRACT_VALUE_HH */
