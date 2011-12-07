#ifndef MIMOSA_TPL_AST_REPEATED_HH
# define MIMOSA_TPL_AST_REPEATED_HH

# include "../../string/string-ref.hh"
# include "node.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      class Repeated : public Node
      {
      public:
        MIMOSA_DEF_PTR(Repeated);

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value,
                             runtime::Time         timeout = 0) const;

        virtual void addChild(Node::Ptr node) { childs_.push(node); }
        virtual string::StringRef var() { return var_; }

        string::StringRef var_;

        nodes_type childs_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_REPEATED_HH */
