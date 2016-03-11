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

        for (auto it = vars_.begin(); it != vars_.end(); ++it)
        {
          v = v->lookup(*it);
          if (!v)
            goto not_found;
        }

        for (auto it = filters_.begin(); it != filters_.end(); ++it)
          stream = FilterFactory::instance().create((std::string)*it, stream);

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
