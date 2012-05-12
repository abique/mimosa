#ifndef MIMOSA_TPL_DICT_HH
# define MIMOSA_TPL_DICT_HH

# include <unordered_map>

# include "abstract-value.hh"
# include "append-helper.hh"

namespace mimosa
{
  namespace tpl
  {
    class Dict : public AbstractValue
    {
    public:
      MIMOSA_DEF_PTR(Dict);

      Dict(const std::string & name = "");

      virtual const AbstractValue * lookup(const StringRef & var) const;
      virtual void write(stream::Stream::Ptr stream, Time timeout = 0) const;

      virtual Iterator::Ptr begin() const;
      virtual bool empty() const;

      typedef std::unordered_map<std::string, AbstractValue::ConstPtr> values_type;

      inline void append(AbstractValue::Ptr value)
      {
        value->parent_ = this;
        values_[value->name()] = value;
      }

      MIMOSA_TPL_APPEND_HELPER

      class DictIterator : public Iterator
      {
      public:
        MIMOSA_DEF_PTR(DictIterator);

        DictIterator(const Dict *                        value,
                     const values_type::const_iterator & it);

        virtual const AbstractValue * value() const;
        virtual void next();
        virtual bool end() const;

      private:
        const Dict *                value_;
        values_type::const_iterator it_;
      };

    private:
      values_type values_;
    };
  }
}

#endif /* !MIMOSA_TPL_DICT_HH */
