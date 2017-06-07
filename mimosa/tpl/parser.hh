#pragma once

# include <string>
# include <vector>

# include "template.hh"
# include "../string-ref.hh"

namespace mimosa
{
  namespace tpl
  {
    class Parser
    {
    public:
      Parser(const Template & tpl);
      ~Parser();

      void reset();

      ast::Root * parse();

      bool parseText();
      bool parseAction();
      bool parseRepeat();
      bool parseRepeatEmpty();
      bool parseEmpty();
      bool parseEnd();
      bool parseExpand();

    private:
      std::string action_start_;
      std::string action_end_;

      StringRef input_;

      std::vector<ast::Node *> stack_;
      int line_;
      int column_;

      ast::Root * root_;
    };
  }
}

