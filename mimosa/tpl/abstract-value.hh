#ifndef MIMOSA_TPL_ABSTRACT_VALUE_HH
# define MIMOSA_TPL_ABSTRACT_VALUE_HH

# include "../ref-countable.hh"
# include "../stream/stream.hh"
# include "../string/string-ref.hh"

namespace mimosa
{
  namespace tpl
  {
    class AbstractValue
    {
    public:
      virtual const AbstractValue * lookup(const string::StringRef & var) const = 0;
      virtual void write(stream::Stream::Ptr stream) const = 0;

      AbstractValue * parent_;
    };
  }
}

#endif /* !MIMOSA_TPL_ABSTRACT_VALUE_HH */
