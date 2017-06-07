#pragma once

# include "../../string-ref.hh"
# include "node.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      class Text : public Node
      {
      public:
        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value) const;


        StringRef text_;
      };
    }
  }
}

