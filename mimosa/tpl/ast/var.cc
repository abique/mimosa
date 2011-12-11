#include "../abstract-value.hh"
#include "var.hh"
#include "../filter-factory.hh"
#include "../../format/format-stream.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      void
      Var::execute(stream::Stream::Ptr   stream,
                   const AbstractValue & value,
                   runtime::Time         timeout) const
      {
        const AbstractValue * v = &value;

        for (auto it = vars_.begin(); it != vars_.end(); ++it)
        {
          v = v->lookup(*it);
          if (!v)
            goto not_found;
        }

        for (auto it = filters_.begin(); it != filters_.end(); ++it)
          stream = FilterFactory::instance().create(*it, stream);

        if (v)
          v->write(stream, timeout);
        else
          not_found:
          format::format(stream, timeout, "(%s not found)", var());
      }

      string::StringRef
      Var::var() const
      {
        if (vars_.empty())
          return "(empty)";
        return vars_.front();
      }
    }
  }
}
