#ifndef MIMOSA_TPL_AST_NODE_HH
# define MIMOSA_TPL_AST_NODE_HH

# include "../../ref-countable.hh"
# include "../../stream/stream.hh"
# include "../../string/string-ref.hh"
# include "../../container/intrusive-slist.hh"

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
                             const AbstractValue & value) const = 0;

        virtual void addChild(Node::Ptr /*node*/) {}
        virtual string::StringRef var() { return "(none)"; }

        typedef container::IntrusiveSlistHook<Node::Ptr>   hook_type;

        hook_type hook_;
        uint16_t  line_;
        uint16_t  col_;

        typedef container::IntrusiveSlist<Node, Node::Ptr, &Node::hook_> nodes_type;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_NODE_HH */
