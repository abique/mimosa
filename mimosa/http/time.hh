#pragma once

# include <string>
# include <ctime>

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    std::string time(::time_t t = ::time(nullptr));

    ::time_t time(const std::string & str);
  }
}

