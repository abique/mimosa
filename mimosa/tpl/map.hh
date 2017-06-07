#pragma once

# include <unordered_map>

namespace mimosa
{
  namespace tpl
  {
    template <typename T>
    class Value : public AbstractValue
    {
    public:
      virtual void write(const StringRef &   var,
                         stream::Stream::Ptr stream);

      std::unordered_map<std::string, Value *> values_;
    };
  }
}

