#pragma once

# include "node.hh"

namespace mimosa
{
  namespace tpl
  {
    class Template;

    namespace ast
    {
      class Root : public Node
      {
      public:
        Root(const Template & tpl);
        ~Root();

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value) const;
        virtual void addChild(Node * node) { childs_.push(node); }

        const Template & tpl_;
        nodes_type       childs_;
      };
    }
  }
}

