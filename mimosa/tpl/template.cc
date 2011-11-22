#include <vector>

#include "../string/string-ref.hh"
#include "../stream/fd-stream.hh"
#include "template.hh"
#include "parser.hh"
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

      Parser parser(*tpl);
      tpl->root_ = parser.parse();
      if (tpl->root_)
        return tpl;
      return nullptr;
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
  }
}
