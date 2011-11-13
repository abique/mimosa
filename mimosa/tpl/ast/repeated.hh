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

      protected:
        string::StringRef var_;

        nodes_type childs_;
        nodes_type empty_var_childs_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_REPEATED_HH */
