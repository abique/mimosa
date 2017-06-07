#pragma once

# include "template.hh"
# include "abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    class Include : public AbstractValue
    {
    public:
      MIMOSA_DEF_PTR(Include);

      Include(const std::string & name,
              Template::ConstPtr  tpl);

      virtual const AbstractValue * lookup(const StringRef & var) const;
      virtual void write(stream::Stream::Ptr stream) const;

      virtual Iterator::Ptr begin() const;
      virtual bool empty() const;

      Template::ConstPtr tpl_;
    };
  }
}

