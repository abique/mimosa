#pragma once

# include <vector>

# include "abstract-value.hh"
# include "append-helper.hh"

namespace mimosa
{
  namespace tpl
  {
    class List : public AbstractValue
    {
    public:
      MIMOSA_DEF_PTR(List);

      explicit List(const std::string & name = "");

      virtual const AbstractValue * lookup(const StringRef & var) const;
      virtual void write(stream::Stream::Ptr stream) const;

      virtual Iterator::Ptr begin() const;
      virtual bool empty() const;

      typedef std::vector<AbstractValue::ConstPtr> values_type;

      inline void append(AbstractValue::Ptr value)
      {
        value->parent_ = this;
        values_.push_back(value.get());
      }

      MIMOSA_TPL_APPEND_HELPER

      class ListIterator : public Iterator
      {
      public:
        MIMOSA_DEF_PTR(ListIterator);

        ListIterator(const List *                        value,
                     const values_type::const_iterator & it);

        virtual const AbstractValue * value() const;
        virtual void next();
        virtual bool end() const;

      private:
        const List *                value_;
        values_type::const_iterator it_;
      };

    private:
      values_type values_;
    };
  }
}

