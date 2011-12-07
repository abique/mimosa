#include "text.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace ast
    {
      void
      Text::execute(stream::Stream::Ptr   stream,
                    const AbstractValue & /*value*/,
                    runtime::Time         timeout) const
      {
        stream->write(text_.data(), text_.size(), timeout);
      }
    }
  }
}
