#include "../abstract-value.hh"
#include "var.hh"
#include "../filter-factory.hh"
#include "../../format/format-stream.hh"
#include "../../options/options.hh"

namespace mimosa
{
  namespace tpl
  {
    auto & TPL_DEBUG = *options::addSwitch("tpl", "tpl-debug", "helps debugging of template");

    namespace ast
    {
      void
      Var::execute(stream::Stream::Ptr   stream,
                   const AbstractValue & value) const
      {
        const AbstractValue * v = &value;

        for (const auto & var : vars_)
        {
          v = v->lookup(var);
          if (!v)
            goto not_found;
        }

        for (const auto & filter : filters_)
          stream = FilterFactory::instance().create((std::string)filter, stream);

        if (v)
          v->write(stream);
        else
          not_found:
          if (TPL_DEBUG)
            format::format(*stream, "(%s not found)", var());
      }

      StringRef
      Var::var() const
      {
        if (vars_.empty())
          return "(empty)";
        return vars_.front();
      }
    }
  }
}
