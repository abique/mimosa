#pragma once

# include <string>

# include "response-writer.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    bool redirect(ResponseWriter & response, const std::string & location);
  }
}

