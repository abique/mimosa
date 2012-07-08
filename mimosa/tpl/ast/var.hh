#ifndef MIMOSA_TPL_AST_VAR_HH
# define MIMOSA_TPL_AST_VAR_HH

# include <vector>

# include "../../string-ref.hh"
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
        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value) const;

        virtual StringRef var() const;

        std::vector<StringRef> vars_;
        std::vector<StringRef> filters_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_VAR_HH */
