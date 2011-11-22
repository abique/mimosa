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

        string::StringRef                var_;
        // XXX filters
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_VAR_HH */
