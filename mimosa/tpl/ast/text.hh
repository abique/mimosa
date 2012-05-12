#ifndef MIMOSA_TPL_AST_TEXT_HH
# define MIMOSA_TPL_AST_TEXT_HH

# include "../../string-ref.hh"
# include "node.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      class Text : public Node
      {
      public:
        MIMOSA_DEF_PTR(Text);

        virtual void execute(stream::Stream::Ptr   stream,
                             const AbstractValue & value,
                             Time         timeout = 0) const;


        StringRef text_;
      };
    }
  }
}

#endif /* !MIMOSA_TPL_AST_TEXT_HH */
