#include <vector>

#include "../string/string-ref.hh"
#include "../stream/fd-stream.hh"
#include "template.hh"
#include "ast/text.hh"

namespace mimosa
{
  namespace tpl
  {
    Template::Ptr
    Template::parseString(const std::string & str)
    {
      Template::Ptr tpl(new Template);

      tpl->data_ = str;
    }

    Template::Ptr
    Template::parseFile(const std::string & path)
    {
      auto stream = stream::FdStream::openFile(path.c_str());
      if (!stream)
        return nullptr;

      std::string data;
      while (true)
      {
        auto buffer = stream->read();
        if (!buffer)
          break;
        data.append(buffer->data(), buffer->size());
      }
      stream = nullptr;

      auto tpl = parseString(data);
      if (tpl)
        tpl->filename_ = path;
      return tpl;
    }

    bool
    Template::parse()
    {
      string::StringRef               input(data_);
      ast::Root::Ptr                  root(new ast::Root(*this));
      std::vector<Node::nodes_type &> stack;

      stack.push_back(root->childs_);
      while (!input.empty())
      {
        auto pos = input.find("{{");
        if (pos > 0)
        {
          // extracts the text
          auto text = new ast::Text;
          text->text_(input.substr(0, pos));
          stack.back().push(text);
          input = input.substr(pos);
        }

        // parse the
      }

      ast::Root::Ptr root(new ast::Root(*this));
    }
  }
}
