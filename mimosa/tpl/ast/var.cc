#include "../abstract-value.hh"
#include "var.hh"
#include "../../format/format-stream.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      void
      Var::execute(stream::Stream::Ptr   stream,
                   const AbstractValue & value,
                   runtime::Time         timeout) const
      {
        auto v = value.lookup(var_);
        if (v)
          v->write(stream, timeout);
        else
          format::format(stream, timeout, "(%s not found)", var_);
      }
    }
  }
}
