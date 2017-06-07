#pragma once

# include <string>

# include "../ref-countable.hh"
# include "ast/root.hh"

namespace mimosa
{
  namespace tpl
  {
    class Parser;
    class Template : public RefCountable<Template>
    {
    public:
      ~Template();

      static Template::Ptr parseString(const std::string & str);
      static Template::Ptr parseFile(const std::string & path);

      void execute(stream::Stream::Ptr   stream,
                   const AbstractValue & value) const;

    private:
      friend class Parser;

      std::string       data_;
      std::string       filename_;
      const ast::Root * root_;
    };
  }
}

