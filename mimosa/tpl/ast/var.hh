#ifndef MIMOSA_TPL_AST_VAR_HH
# define MIMOSA_TPL_AST_VAR_HH

# include <vector>

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

        virtual string::StringRef var() const;

        std::vector<string::StringRef> vars_;
        std::vector<string::StringRef> filters_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_VAR_HH */
