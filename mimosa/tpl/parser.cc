#include "log.hh"
#include "parser.hh"
#include "ast/text.hh"
#include "ast/var.hh"
#include "ast/repeated.hh"
#include "ast/empty.hh"

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

    Parser::~Parser()
    {
      delete root_;
    }

    ast::Root *
    Parser::parse()
    {
      if (action_start_.empty() || action_end_.empty())
        return nullptr;

      stack_.push_back(root_);
      auto succeed = parseText();
      auto root    = root_;
      root_        = nullptr;

      if (!succeed) {
        delete root;
        return nullptr;
      }
      return root;
    }

    bool
    Parser::parseText()
    {
      while (!input_.empty())
      {
        auto pos = input_.find(action_start_);
        if (pos > 0 || pos == StringRef::npos)
        {
          // extracts the text
          auto text = new ast::Text;
          text->text_ = input_.substr(0, pos);
          stack_.back()->addChild(text);
          input_ = input_.substr(pos);
        }

        if (pos != StringRef::npos &&
            !parseAction())
          return false;
      }

      return true;
    }

    bool
    Parser::parseAction()
    {
      if (input_.size() < action_start_.size() + action_end_.size())
        return false;

      input_ = input_.substr(action_start_.size());

      switch (input_[0])
      {
      case '*':
        if (input_.size() > 1 && input_[1] == '!')
          return parseRepeatEmpty();
        else
          return parseRepeat();

      case '!':
        return parseEmpty();

      case '/':
        return parseEnd();

      default:
        return parseExpand();
      }
    }

    bool
    Parser::parseExpand()
    {
      // find the end of the action
      auto end = input_.find(action_end_);
      if (end == StringRef::npos)
        return false;

      // don't allow {{|x|y|z}}
      if (end == 0 || input_[0] == '|')
        return false;

      // create the var
      auto var = new ast::Var;

      // split with |
      var->filters_ = input_.substr(0, end).tokens('|');

      // split the a.b.c.d
      var->vars_ = var->filters_[0].tokens('.');
      if (var->vars_.empty())
        var->vars_.push_back(".");

      // remove a.b.c.d from the filters
      var->filters_.erase(var->filters_.begin());

      stack_.back()->addChild(var);
      input_ = input_.substr(end + action_end_.size());
      return true;
    }

    bool
    Parser::parseRepeat()
    {
      // skip the '*'
      input_ = input_.substr(1);

      // find the end of the action
      auto end = input_.find(action_end_);
      if (end == StringRef::npos)
        return false;

      auto node = new ast::Repeated;
      node->var_ = input_.substr(0, end);
      stack_.back()->addChild(node);
      stack_.push_back(node);

      input_ = input_.substr(end + action_end_.size());
      return true;
    }

    bool
    Parser::parseRepeatEmpty()
    {
      // skip the '*!'
      input_ = input_.substr(2);

      // find the end of the action
      auto end = input_.find(action_end_);
      if (end == StringRef::npos)
        return false;

      auto node = new ast::Empty;
      node->var_ = input_.substr(0, end);

      StringRef ref_var = stack_.back()->var();

      if (node->var_.empty())
        node->var_ = ref_var;
      else if ((node->var_) != ref_var)
      {
        tpl_log->error("different variables: *%s *!%s", ref_var, node->var_);
        return false;
      }

      stack_.pop_back();
      stack_.back()->addChild(node);
      stack_.push_back(node);

      input_ = input_.substr(end + action_end_.size());
      return true;
    }

    bool
    Parser::parseEmpty()
    {
      // skip the '!'
      input_ = input_.substr(1);

      // find the end of the action
      auto end = input_.find(action_end_);
      if (end == StringRef::npos)
        return false;

      auto node = new ast::Empty;
      node->var_ = input_.substr(0, end);
      stack_.back()->addChild(node);
      stack_.push_back(node);

      input_ = input_.substr(end + action_end_.size());
      return true;
    }

    bool
    Parser::parseEnd()
    {
      // skip the '/'
      input_ = input_.substr(1);

      // find the end of the action
      auto end = input_.find(action_end_);
      if (end == StringRef::npos)
        return false;
      input_ = input_.substr(end + action_end_.size());

      stack_.pop_back();
      return true;
    }
  }
}
