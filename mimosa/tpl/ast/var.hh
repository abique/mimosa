#ifndef MIMOSA_TPL_AST_VAR_HH
# define MIMOSA_TPL_AST_VAR_HH

# include "../../string/string-ref.hh"
# include "../../stream/stream.hh"
# include "node.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      class Var : public Node
      {
      public:
        MIMOSA_DEF_PTR(Var);

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value,
                             runtime::Time         timeout = 0) const;

        virtual string::StringRef var() { return var_; }

        string::StringRef var_;
        // XXX filters
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_VAR_HH */
