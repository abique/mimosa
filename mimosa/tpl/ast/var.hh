#ifndef MIMOSA_TPL_AST_VAR_HH
# define MIMOSA_TPL_AST_VAR_HH

# include "../../string/string-ref.hh"
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

      protected:
        string::StringRef var_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_VAR_HH */
