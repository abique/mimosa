#ifndef MIMOSA_TPL_ABSTRACT_VALUE_HH
# define MIMOSA_TPL_ABSTRACT_VALUE_HH

# include "../ref-countable.hh"
# include "../stream/stream.hh"
# include "../string/string-ref.hh"

namespace mimosa
{
  namespace tpl
  {
    class AbstractValue
    {
    public:
      AbstractValue(const AbstractValue * parent = nullptr);

      virtual const AbstractValue * lookup(const string::StringRef & var) const = 0;
      virtual void write(stream::Stream::Ptr stream, runtime::Time timeout = 0) const = 0;

      class Iterator : public RefCountable<Iterator>
      {
      public:
        virtual const AbstractValue * value() const = 0;
        virtual void next() = 0;
        virtual bool end() const = 0;
      };

      class SingleValueIterator : public Iterator
      {
      public:
        MIMOSA_DEF_PTR(SingleValueIterator);

        SingleValueIterator(const AbstractValue * value = nullptr);

        virtual const AbstractValue * value() const;
        virtual void next();
        virtual bool end() const;

      private:
        const AbstractValue * value_;
      };

      virtual Iterator::Ptr begin() const = 0;
      virtual bool empty() const = 0;

      const AbstractValue * parent_;
    };
  }
}

#endif /* !MIMOSA_TPL_ABSTRACT_VALUE_HH */
