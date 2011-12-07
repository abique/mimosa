#ifndef MIMOSA_TPL_VALUE_LIST_HH
# define MIMOSA_TPL_LIST_HH

# include <vector>

# include "abstract-value.hh"

namespace mimosa
{
  namespace tpl
  {
    class List : public AbstractValue
    {
    public:
      MIMOSA_DEF_PTR(List);

      List(const std::string &   name = "");

      virtual const AbstractValue * lookup(const string::StringRef & var) const;
      virtual void write(stream::Stream::Ptr stream, runtime::Time timeout = 0) const;

      virtual Iterator::Ptr begin() const;
      virtual bool empty() const;

      typedef std::vector<AbstractValue::ConstPtr> values_type;

      inline void append(AbstractValue::Ptr value)
      {
        value->parent_ = this;
        values_.push_back(value);
      }

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

#endif /* !MIMOSA_TPL_LIST_HH */
