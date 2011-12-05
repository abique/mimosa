#include "root.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      void
      Root::execute(stream::Stream::Ptr   stream,
                    const AbstractValue & value) const
      {
        for (auto it = childs_.begin();
             it != childs_.end(); ++it)
          it->execute(stream, value);
      }
    }
  }
}
