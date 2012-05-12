#ifndef MIMOSA_TPL_MAP_HH
# define MIMOSA_TPL_MAP_HH

# include <unordered_map>

namespace mimosa
{
  namespace tpl
  {
    template <typename T>
    class Value : public AbstractValue
    {
    public:
      virtual void write(const StringRef & var,
                         stream::Stream::Ptr       stream);

      std::unordered_map<std::string, Value *> values_;
    };
  }
}

#endif /* !MAP_HH */
