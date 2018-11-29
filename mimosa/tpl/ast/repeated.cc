#include "repeated.hh"
#include "../abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      Repeated::~Repeated()
      {
        while (!childs_.empty()) {
          auto child = childs_.front();
          childs_.pop();
          delete child;
        }
      }

      void
      Repeated::execute(stream::Stream::Ptr   stream,
                        const AbstractValue & value) const
      {
        auto v = value.lookup(var_);
        if (!v || v->empty())
          return;

        for (auto it = v->begin(); !it->end(); it->next())
        {
          for (auto & child : childs_)
            if (it->value())
              child.execute(stream, *it->value());
        }
      }
    }
  }
}
