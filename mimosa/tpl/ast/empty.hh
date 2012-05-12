#ifndef MIMOSA_TPL_AST_EMPTY_HH
# define MIMOSA_TPL_AST_EMPTY_HH

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
        MIMOSA_DEF_PTR(Empty);

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value,
                             Time         timeout = 0) const;

        virtual void addChild(Node::Ptr node) { childs_.push(node); }
        virtual StringRef var() const { return var_; }

        StringRef var_;

        nodes_type childs_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_EMPTY_HH */
