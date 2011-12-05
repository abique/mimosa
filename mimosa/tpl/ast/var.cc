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
                   const AbstractValue & value) const
      {
        auto v = value.lookup(var_);
        if (v)
          v->write(stream);
        else
          format::format(stream, "(%s not found)", var_);
      }
    }
  }
}
