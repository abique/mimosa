#ifndef MIMOSA_TPL_AST_EMPTY_HH
# define MIMOSA_TPL_AST_EMPTY_HH

# include "../../string/string-ref.hh"
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

        string::StringRef var_;

        nodes_type childs_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_EMPTY_HH */
