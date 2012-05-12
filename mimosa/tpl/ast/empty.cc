#include "empty.hh"
#include "../abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      void
      Empty::execute(stream::Stream::Ptr   stream,
                     const AbstractValue & value,
                     Time         timeout) const
      {
        auto v = value.lookup(var_);
        if (v && !v->empty())
          return;

        if (!v)
          v = &value;

        for (auto child = childs_.begin(); child != childs_.end(); ++child)
          child->execute(stream, *v, timeout);
      }
    }
  }
}
