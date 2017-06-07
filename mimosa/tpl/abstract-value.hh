#pragma once

# include "../ref-countable.hh"
# include "../stream/stream.hh"
# include "../string-ref.hh"

namespace mimosa
{
  namespace tpl
  {
    class AbstractValue : public RefCountable<AbstractValue>
    {
    public:
      AbstractValue(const std::string & name = "");

      virtual const AbstractValue * lookup(const StringRef & var) const = 0;
      virtual void write(stream::Stream::Ptr stream) const = 0;

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

      inline const std::string & name() const { return name_; }

      const AbstractValue * parent_;
      std::string           name_;
    };
  }
}

