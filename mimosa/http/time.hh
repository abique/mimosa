#ifndef MIMOSA_HTTP_TIME_HH
# define MIMOSA_HTTP_TIME_HH

# include <string>
# include <ctime>

namespace mimosa
{
  namespace http
  {
    std::string time(::time_t t = ::time(nullptr));
  }
}

#endif /* !MIMOSA_HTTP_TIME_HH */
