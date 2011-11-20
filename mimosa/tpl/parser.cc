#include "parser.hh"

namespace mimosa
{
  namespace tpl
  {
    Parser::Parser(const Template & tpl)
      : action_start_("{{"),
        action_end_("}}"),
        input_(tpl.data_),
        stack_(),
        line_(1),
        column_(0),
        root_(new ast::Root(tpl))
    {
    }

    ast::Root::Ptr
    Parser::parse()
    {
      stack_.push_back(root_);
    }

    bool
    Parser::parseText()
    {
      while (!input_.empty())
      {
        if (pos > 0 || pos == string::StringRef::npos)
        {
          // extracts the text
          auto text = new ast::Text;
          text->text_(input_.substr(0, pos));
          stack_.back().push(text);
          input = input_.substr(pos);
        }

        parseAction();
      }

      return true;
    }
  }
}
