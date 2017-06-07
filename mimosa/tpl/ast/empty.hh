#pragma once

# include "../../string-ref.hh"
# include "node.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      class Empty : public Node
      {
      public:
        ~Empty();

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value) const;

        virtual void addChild(Node * node) { childs_.push(node); }
        virtual StringRef var() const { return var_; }

        StringRef var_;

        nodes_type childs_;
      };
    }
  }
}

