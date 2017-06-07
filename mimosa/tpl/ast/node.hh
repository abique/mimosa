#pragma once

# include "../../ref-countable.hh"
# include "../../stream/stream.hh"
# include "../../string-ref.hh"
# include "../../intrusive-slist.hh"

namespace mimosa
{
  namespace tpl
  {
    class AbstractValue;

    namespace ast
    {
      class Node
      {
      public:
        virtual ~Node() {}

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value) const = 0;

        virtual void addChild(Node * /*node*/) {}
        virtual StringRef var() const { return "(none)"; }

        typedef IntrusiveSlistHook<Node *> hook_type;

        hook_type hook_;
        uint16_t  line_;
        uint16_t  col_;

        typedef IntrusiveSlist<Node, Node *, &Node::hook_> nodes_type;
      };
    }
  }
}

