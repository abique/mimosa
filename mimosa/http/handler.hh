#pragma once

#include "../ref-countable.hh"
#include "../non-copyable.hh"
#include "../non-movable.hh"
#include "request-reader.hh"
#include "response-writer.hh"

namespace mimosa
{
  namespace http
  {
    class Request;

    /**
     * @ingroup Http
     */
    class Handler : public RefCountable<Handler>, private NonCopyable, private NonMovable
    {
    public:
      virtual ~Handler() {}
      virtual bool handle(RequestReader & request, ResponseWriter & response) const = 0;
    };
  }
}

