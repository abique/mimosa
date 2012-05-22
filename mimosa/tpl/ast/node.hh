#ifndef MIMOSA_TPL_AST_NODE_HH
# define MIMOSA_TPL_AST_NODE_HH

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
      class Node : public RefCountable<Node>
      {
      public:

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value,
                             Time         timeout = 0) const = 0;

        virtual void addChild(Node::Ptr /*node*/) {}
        virtual StringRef var() const { return "(none)"; }

        typedef IntrusiveSlistHook<Node::Ptr>   hook_type;

        hook_type hook_;
        uint16_t  line_;
        uint16_t  col_;

        typedef IntrusiveSlist<Node, Node::Ptr, &Node::hook_> nodes_type;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_NODE_HH */
