#include "empty.hh"
#include "../abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      Empty::~Empty()
      {
        while (!childs_.empty()) {
          auto child = childs_.front();
          childs_.pop();
          delete child;
        }
      }

      void
      Empty::execute(stream::Stream::Ptr   stream,
                     const AbstractValue & value) const
      {
        auto v = value.lookup(var_);
        if (v && !v->empty())
          return;

        if (!v)
          v = &value;

        for (auto & child : childs_)
          child.execute(stream, *v);
      }
    }
  }
}
